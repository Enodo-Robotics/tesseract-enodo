/**
 * @file raster_waad_dt_process_manager.cpp
 * @brief Plans raster paths with approach, departure and dual transitions
 *
 * @author Levi Armstrong
 * @date August 28, 2020
 * @version TODO
 * @bug No known bugs
 *
 * @copyright Copyright (c) 2020, Southwest Research Institute
 *
 * @par License
 * Software License Agreement (Apache License)
 * @par
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * @par
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <functional>
#include <taskflow/taskflow.hpp>
#include <fstream>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_process_managers/process_managers/raster_waad_dt_process_manager.h>
#include <tesseract_command_language/instruction_type.h>
#include <tesseract_command_language/composite_instruction.h>
#include <tesseract_command_language/plan_instruction.h>
#include <tesseract_command_language/utils/get_instruction_utils.h>

using namespace tesseract_planning;

RasterWAADDTProcessManager::RasterWAADDTProcessManager(TaskflowGenerator::UPtr freespace_taskflow_generator,
                                                       TaskflowGenerator::UPtr transition_taskflow_generator,
                                                       TaskflowGenerator::UPtr raster_taskflow_generator,
                                                       std::size_t n)
  : freespace_taskflow_generator_(std::move(freespace_taskflow_generator))
  , transition_taskflow_generator_(std::move(transition_taskflow_generator))
  , raster_taskflow_generator_(std::move(raster_taskflow_generator))
  , executor_(n)
  , taskflow_("RasterWAADDTProcessManagerTaskflow")
{
}

bool RasterWAADDTProcessManager::init(ProcessInput input)
{
  // This should make all of the isComposite checks so that you can safely cast below
  if (!checkProcessInput(input))
  {
    CONSOLE_BRIDGE_logError("Invalid Process Input");
    return false;
  }

  // Clear the process manager
  clear();

  // Store the current size of the tasks so that we can add from_start later
  std::size_t starting_raster_idx = raster_tasks_.size();

  // Generate all of the raster tasks. They don't depend on anything
  for (std::size_t idx = 1; idx < input.size() - 1; idx += 2)
  {
    // Get the last plan instruction of the approach
    assert(isCompositeInstruction(*(input[idx][0].instruction)));
    const auto* aci = input[idx][0].instruction->cast_const<CompositeInstruction>();
    auto* ali = getLastPlanInstruction(*aci);
    assert(ali != nullptr);

    // Create the process taskflow
    ProcessInput process_input = input[idx][1];
    process_input.start_instruction = *ali;
    auto process_step =
        taskflow_
            .composed_of(raster_taskflow_generator_->generateTaskflow(
                process_input,
                std::bind(
                    &RasterWAADDTProcessManager::successCallback, this, process_input.instruction->getDescription()),
                std::bind(
                    &RasterWAADDTProcessManager::failureCallback, this, process_input.instruction->getDescription())))
            .name("raster_" + std::to_string(idx));

    // Create Departure Taskflow
    ProcessInput departure_input = input[idx][2];
    departure_input.start_instruction_ptr = input[idx][1].results;
    auto departure_step =
        taskflow_
            .composed_of(raster_taskflow_generator_->generateTaskflow(
                departure_input,
                std::bind(
                    &RasterWAADDTProcessManager::successCallback, this, departure_input.instruction->getDescription()),
                std::bind(
                    &RasterWAADDTProcessManager::failureCallback, this, departure_input.instruction->getDescription())))
            .name("departure_" + std::to_string(idx));

    // Get Start Plan Instruction for approach
    Instruction start_instruction = NullInstruction();
    if (idx == 1)
    {
      assert(isCompositeInstruction(*(input[0].instruction)));
      const auto* ci = input[0].instruction->cast_const<CompositeInstruction>();
      const auto* li = getLastPlanInstruction(*ci);
      assert(li != nullptr);
      start_instruction = *li;
    }
    else
    {
      assert(isCompositeInstruction(*(input[idx - 1].instruction)));
      const auto* tci = input[idx - 1].instruction->cast_const<CompositeInstruction>();
      auto* li = getLastPlanInstruction(*tci);
      assert(li != nullptr);
      start_instruction = *li;
    }

    // Create the departure taskflow
    start_instruction.cast<PlanInstruction>()->setPlanType(PlanInstructionType::START);
    ProcessInput approach_input = input[idx][0];
    approach_input.start_instruction = start_instruction;
    approach_input.end_instruction_ptr = input[idx][1].results;
    auto approach_step =
        taskflow_
            .composed_of(raster_taskflow_generator_->generateTaskflow(
                approach_input,
                std::bind(
                    &RasterWAADDTProcessManager::successCallback, this, approach_input.instruction->getDescription()),
                std::bind(
                    &RasterWAADDTProcessManager::failureCallback, this, approach_input.instruction->getDescription())))
            .name("approach_" + std::to_string(idx));

    // Each approach and departure depend on raster
    approach_step.succeed(process_step);
    departure_step.succeed(process_step);

    raster_tasks_.push_back(std::array<tf::Task, 3>({ approach_step, process_step, departure_step }));
  }

  // Loop over all transitions
  std::size_t transition_idx = 0;
  for (std::size_t input_idx = 2; input_idx < input.size() - 2; input_idx += 2)
  {
    // This use to extract the start and end, but things were changed so the seed is generated as part of the
    // taskflow. So the seed is only a skeleton and does not contain move instructions. So instead we provide the
    // composite and let the generateTaskflow extract the start and end waypoint from the composite. This is also more
    // robust because planners could modify composite size, which is rare but does happen when using OMPL where it is
    // not possible to simplify the trajectory to the desired number of states.
    ProcessInput transition_from_end_input = input[input_idx][0];
    transition_from_end_input.start_instruction_ptr = input[input_idx - 1][2].results;
    transition_from_end_input.end_instruction_ptr = input[input_idx + 1][0].results;
    auto transition_from_end_step = taskflow_
                                        .composed_of(transition_taskflow_generator_->generateTaskflow(
                                            transition_from_end_input,
                                            std::bind(&RasterWAADDTProcessManager::successCallback,
                                                      this,
                                                      transition_from_end_input.instruction->getDescription()),
                                            std::bind(&RasterWAADDTProcessManager::failureCallback,
                                                      this,
                                                      transition_from_end_input.instruction->getDescription())))
                                        .name("transition_" + std::to_string(input_idx));

    // Each transition is independent and thus depends only on the adjacent rasters approach and departure
    transition_from_end_step.succeed(raster_tasks_[starting_raster_idx + transition_idx][2]);
    transition_from_end_step.succeed(raster_tasks_[starting_raster_idx + transition_idx + 1][0]);

    transition_tasks_.push_back(transition_from_end_step);

    ProcessInput transition_to_start_input = input[input_idx][1];
    transition_to_start_input.start_instruction_ptr = input[input_idx + 1][2].results;
    transition_to_start_input.end_instruction_ptr = input[input_idx - 1][0].results;
    auto transition_to_start_step = taskflow_
                                        .composed_of(transition_taskflow_generator_->generateTaskflow(
                                            transition_to_start_input,
                                            std::bind(&RasterWAADDTProcessManager::successCallback,
                                                      this,
                                                      transition_to_start_input.instruction->getDescription()),
                                            std::bind(&RasterWAADDTProcessManager::failureCallback,
                                                      this,
                                                      transition_to_start_input.instruction->getDescription())))
                                        .name("transition_" + std::to_string(input_idx));

    // Each transition is independent and thus depends only on the adjacent rasters approach and departure
    transition_to_start_step.succeed(raster_tasks_[starting_raster_idx + transition_idx][2]);
    transition_to_start_step.succeed(raster_tasks_[starting_raster_idx + transition_idx + 1][0]);

    transition_tasks_.push_back(transition_to_start_step);

    transition_idx++;
  }

  // Plan from_start - preceded by the first raster
  ProcessInput from_start_input = input[0];
  from_start_input.start_instruction = input.instruction->cast_const<CompositeInstruction>()->getStartInstruction();
  from_start_input.end_instruction_ptr = input[1][0].results;
  auto from_start =
      taskflow_
          .composed_of(freespace_taskflow_generator_->generateTaskflow(
              from_start_input,
              std::bind(
                  &RasterWAADDTProcessManager::successCallback, this, from_start_input.instruction->getDescription()),
              std::bind(
                  &RasterWAADDTProcessManager::failureCallback, this, from_start_input.instruction->getDescription())))
          .name("from_start");
  raster_tasks_[starting_raster_idx][0].precede(from_start);
  freespace_tasks_.push_back(from_start);

  // Plan to_end - preceded by the last raster
  ProcessInput to_end_input = input[input.size() - 1];
  to_end_input.start_instruction_ptr = input[input.size() - 2][2].results;
  auto to_end =
      taskflow_
          .composed_of(freespace_taskflow_generator_->generateTaskflow(
              to_end_input,
              std::bind(&RasterWAADDTProcessManager::successCallback, this, to_end_input.instruction->getDescription()),
              std::bind(
                  &RasterWAADDTProcessManager::failureCallback, this, to_end_input.instruction->getDescription())))
          .name("to_end");
  raster_tasks_.back()[2].precede(to_end);
  freespace_tasks_.push_back(to_end);

  // visualizes the taskflow
  std::ofstream out_data;
  out_data.open("/tmp/raster_waad_dt_process_manager.dot");
  taskflow_.dump(out_data);
  out_data.close();

  return true;
}

bool RasterWAADDTProcessManager::execute()
{
  success_ = true;

  // TODO: Figure out how to cancel execution. This callback is only checked at beginning of the taskflow (ie before
  // restarting)
  //  executor.run_until(taskflow, [this]() { std::cout << "Checking if done: " << this->done << std::endl; return
  //  this->done;});

  // Wait for currently running taskflows to end.
  executor_.wait_for_all();
  executor_.run(taskflow_).wait();

  return success_;
}

bool RasterWAADDTProcessManager::terminate()
{
  freespace_taskflow_generator_->abort();
  transition_taskflow_generator_->abort();
  raster_taskflow_generator_->abort();

  CONSOLE_BRIDGE_logError("Terminating Taskflow");
  return false;
}

bool RasterWAADDTProcessManager::clear()

{
  freespace_taskflow_generator_->clear();
  transition_taskflow_generator_->clear();
  raster_taskflow_generator_->clear();
  taskflow_.clear();
  freespace_tasks_.clear();
  raster_tasks_.clear();
  return true;
}

bool RasterWAADDTProcessManager::checkProcessInput(const tesseract_planning::ProcessInput& input) const
{
  // -------------
  // Check Input
  // -------------
  if (!input.tesseract)
  {
    CONSOLE_BRIDGE_logError("ProcessInput tesseract is a nullptr");
    return false;
  }

  // Check the overall input
  if (!isCompositeInstruction(*(input.instruction)))
  {
    CONSOLE_BRIDGE_logError("ProcessInput Invalid: input.instructions should be a composite");
    return false;
  }
  const auto* composite = input.instruction->cast_const<CompositeInstruction>();

  // Check that it has a start instruction
  if (!composite->hasStartInstruction() && input.start_instruction_ptr == nullptr &&
      isNullInstruction(input.start_instruction))
  {
    CONSOLE_BRIDGE_logError("ProcessInput Invalid: input.instructions should have a start instruction");
    return false;
  }

  // Check from_start
  if (!isCompositeInstruction(composite->at(0)))
  {
    CONSOLE_BRIDGE_logError("ProcessInput Invalid: from_start should be a composite");
    return false;
  }

  // Check rasters and transitions
  for (std::size_t index = 1; index < composite->size() - 1; index++)
  {
    // Both rasters and transitions should be a composite
    if (!isCompositeInstruction(composite->at(index)))
    {
      CONSOLE_BRIDGE_logError("ProcessInput Invalid: Both rasters and transitions should be a composite");
      return false;
    }

    // Convert to composite
    const auto* step = composite->at(index).cast_const<CompositeInstruction>();

    // Odd numbers are raster segments
    if (index % 2 == 1)
    {
      // Raster must have three composites approach, raster and departure
      if (step->size() != 3)
      {
        CONSOLE_BRIDGE_logError("ProcessInput Invalid: Rasters must have three element approach, raster and departure");
        return false;
      }
      // The approach should be a composite
      if (!isCompositeInstruction(step->at(0)))
      {
        CONSOLE_BRIDGE_logError("ProcessInput Invalid: The raster approach should be a composite");
        return false;
      }

      // The process should be a composite
      if (!isCompositeInstruction(step->at(1)))
      {
        CONSOLE_BRIDGE_logError("ProcessInput Invalid: The process should be a composite");
        return false;
      }

      // The departure should be a composite
      if (!isCompositeInstruction(step->at(2)))
      {
        CONSOLE_BRIDGE_logError("ProcessInput Invalid: The departure should be a composite");
        return false;
      }
    }
    else
    {
      // Evens are transitions
      // If there is only one transition, we assume it is transition_from_end
      if (step->size() == 2)
      {
        // If there are multiple, then they should be unordered
        if (step->getOrder() != CompositeInstructionOrder::UNORDERED)
        {
          // If you get this error, check that this is not processing a raster strip. You may be missing from_start.
          CONSOLE_BRIDGE_logError("Raster contains multiple transitions but is not marked UNORDERED");
          step->print();
          return false;
        }

        if (!isCompositeInstruction(step->at(0)))
        {
          CONSOLE_BRIDGE_logError("ProcessInput Invalid: transition from end should be a composite");
          return false;
        }

        if (!isCompositeInstruction(step->at(1)))
        {
          CONSOLE_BRIDGE_logError("ProcessInput Invalid: transition to start should be a composite");
          return false;
        }
      }
      else
      {
        CONSOLE_BRIDGE_logError("ProcessInput Invalid: transition should be a composite of size 2");
        return false;
      }
    }
  }

  // Check to_end
  if (!isCompositeInstruction(composite->back()))
  {
    CONSOLE_BRIDGE_logError("ProcessInput Invalid: to_end should be a composite");
    return false;
  };

  return true;
}

void RasterWAADDTProcessManager::successCallback(std::string message)
{
  CONSOLE_BRIDGE_logInform("RasterWAADDTProcessManager Successful: %s", message.c_str());
  success_ &= true;
}

void RasterWAADDTProcessManager::failureCallback(std::string message)
{
  // For this process, any failure of a sub-TaskFlow indicates a planning failure. Abort all future tasks
  freespace_taskflow_generator_->abort();
  transition_taskflow_generator_->abort();
  raster_taskflow_generator_->abort();
  // Print an error if this is the first failure
  if (success_)
    CONSOLE_BRIDGE_logError("RasterWAADDTProcessManager Failure: %s", message.c_str());
  success_ = false;
}