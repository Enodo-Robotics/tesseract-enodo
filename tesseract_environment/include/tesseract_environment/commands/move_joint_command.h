/**
 * @file move_joint_command.h
 * @brief Used to move joint in environment
 *
 * @author Levi Armstrong
 * @date Dec 18, 2017
 * @version TODO
 * @bug No known bugs
 *
 * @copyright Copyright (c) 2017, Southwest Research Institute
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
#ifndef TESSERACT_ENVIRONMENT_MOVE_JOINT_COMMAND_H
#define TESSERACT_ENVIRONMENT_MOVE_JOINT_COMMAND_H

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
class MoveJointCommand : public Command
{
public:
  using Ptr = std::shared_ptr<MoveJointCommand>;
  using ConstPtr = std::shared_ptr<const MoveJointCommand>;

  MoveJointCommand();

  /**
   * @brief Move a joint from one link to another
   *
   *        All child links & joints should follow
   *
   * @param joint_name The name of the joint to move
   * @param new_parent_link The name of the link to move to.e
   */
  MoveJointCommand(std::string joint_name, std::string parent_link);

  const std::string& getJointName() const;
  const std::string& getParentLink() const;

  bool operator==(const MoveJointCommand& rhs) const;
  bool operator!=(const MoveJointCommand& rhs) const;

private:
  std::string joint_name_;
  std::string parent_link_;

  friend class boost::serialization::access;
  friend struct tesseract_common::Serialization;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version);  // NOLINT
};
}  // namespace tesseract_environment

BOOST_CLASS_EXPORT_KEY(tesseract_environment::MoveJointCommand)

#endif  // TESSERACT_ENVIRONMENT_MOVE_JOINT_COMMAND_H
