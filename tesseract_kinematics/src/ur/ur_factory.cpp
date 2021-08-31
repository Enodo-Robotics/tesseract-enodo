/**
 * @file ur_factory.cpp
 * @brief Tesseract UR Inverse kinematics Factory
 *
 * @author Levi Armstrong
 * @date Aug 27, 2021
 * @version TODO
 * @bug No known bugs
 *
 * @copyright Copyright (c) 2021, Southwest Research Institute
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

#include <tesseract_kinematics/ur/ur_factory.h>
#include <tesseract_kinematics/ur/ur_inv_kin.h>
#include <tesseract_kinematics/core/types.h>

namespace tesseract_kinematics
{
InverseKinematics::UPtr URInvKinFactory::create(const std::string& name,
                                                const tesseract_scene_graph::SceneGraph& scene_graph,
                                                const tesseract_scene_graph::SceneState& /*scene_state*/,
                                                const KinematicsPluginFactory& /*plugin_factory*/,
                                                const YAML::Node& config) const
{
  std::string base_link;
  std::string tip_link;
  tesseract_kinematics::URParameters params;
  tesseract_scene_graph::ShortestPath path;

  try
  {
    base_link = config["base_link"].as<std::string>();
    tip_link = config["tip_link"].as<std::string>();
    if (YAML::Node model = config["model"])
    {
      int model_num = model.as<int>();
      if (model_num == 3)
        params = UR3Parameters;
      else if (model_num == 5)
        params = UR5Parameters;
      else if (model_num == 10)
        params = UR10Parameters;
      else
      {
        CONSOLE_BRIDGE_logError("URInvKinFactory: Invalid model number!");
        return nullptr;
      }
    }
    else
    {
      const YAML::Node& ur_params = config["params"];
      params.d1 = ur_params["d1"].as<double>();
      params.a2 = ur_params["a2"].as<double>();
      params.a3 = ur_params["a3"].as<double>();
      params.d4 = ur_params["d4"].as<double>();
      params.d5 = ur_params["d5"].as<double>();
      params.d6 = ur_params["d6"].as<double>();
    }

    path = scene_graph.getShortestPath(base_link, tip_link);
  }
  catch (...)
  {
    CONSOLE_BRIDGE_logError("URInvKinFactory: Failed to parse yaml config data!");
    return nullptr;
  }

  return std::make_unique<URInvKin>(name, params, base_link, tip_link, path.active_joints);
}
}  // namespace tesseract_kinematics

TESSERACT_ADD_PLUGIN(tesseract_kinematics::URInvKinFactory, URInvKinFactory);