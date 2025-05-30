/**
 * @file move_link_command.h
 * @brief Used to move link in environment
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
#ifndef TESSERACT_ENVIRONMENT_MOVE_LINK_COMMAND_H
#define TESSERACT_ENVIRONMENT_MOVE_LINK_COMMAND_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <memory>
#include <boost/serialization/export.hpp>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_environment/command.h>
#include <tesseract_scene_graph/fwd.h>

namespace boost::serialization
{
class access;
}

namespace tesseract_environment
{
class MoveLinkCommand : public Command
{
public:
  using Ptr = std::shared_ptr<MoveLinkCommand>;
  using ConstPtr = std::shared_ptr<const MoveLinkCommand>;

  MoveLinkCommand();

  /**
   * @brief Move a link in the environment
   *
   *        This should delete the parent joint of the child link. All child links and joints follow.
   *
   * @param joint The new joint.
   */
  MoveLinkCommand(const tesseract_scene_graph::Joint& joint);

  const std::shared_ptr<const tesseract_scene_graph::Joint>& getJoint() const;

  bool operator==(const MoveLinkCommand& rhs) const;
  bool operator!=(const MoveLinkCommand& rhs) const;

private:
  std::shared_ptr<const tesseract_scene_graph::Joint> joint_;

  friend class boost::serialization::access;
  friend struct tesseract_common::Serialization;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version);  // NOLINT
};
}  // namespace tesseract_environment

BOOST_CLASS_EXPORT_KEY(tesseract_environment::MoveLinkCommand)
#endif  // TESSERACT_ENVIRONMENT_MOVE_LINK_COMMAND_H
