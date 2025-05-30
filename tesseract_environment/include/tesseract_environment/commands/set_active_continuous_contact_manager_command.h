/**
 * @file set_active_continuous_contact_manager_command.h
 * @brief Used to set the active continuous contact manager for the environment
 *
 * @author Levi Armstrong
 * @date October 26, 2021
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
#ifndef TESSERACT_ENVIRONMENT_SET_ACTIVE_CONTINUOUS_CONTACT_MANAGER_COMMAND_H
#define TESSERACT_ENVIRONMENT_SET_ACTIVE_CONTINUOUS_CONTACT_MANAGER_COMMAND_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <memory>
#include <string>
#include <boost/serialization/export.hpp>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_environment/command.h>

namespace boost::serialization
{
class access;
}

namespace tesseract_environment
{
class SetActiveContinuousContactManagerCommand : public Command
{
public:
  using Ptr = std::shared_ptr<SetActiveContinuousContactManagerCommand>;
  using ConstPtr = std::shared_ptr<const SetActiveContinuousContactManagerCommand>;

  SetActiveContinuousContactManagerCommand();

  /**
   * @brief Set the active discrete contact manager
   * @param active_contact_manager Active contact manager name
   */
  SetActiveContinuousContactManagerCommand(std::string active_contact_manager);

  const std::string& getName() const;

  bool operator==(const SetActiveContinuousContactManagerCommand& rhs) const;
  bool operator!=(const SetActiveContinuousContactManagerCommand& rhs) const;

private:
  std::string active_contact_manager_;

  friend class boost::serialization::access;
  friend struct tesseract_common::Serialization;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version);  // NOLINT
};
}  // namespace tesseract_environment

BOOST_CLASS_EXPORT_KEY(tesseract_environment::SetActiveContinuousContactManagerCommand)
#endif  // TESSERACT_ENVIRONMENT_SET_ACTIVE_CONTINUOUS_CONTACT_MANAGER_COMMAND_H
