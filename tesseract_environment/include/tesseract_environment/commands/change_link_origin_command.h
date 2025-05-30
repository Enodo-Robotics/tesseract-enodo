/**
 * @file change_link_origin_command.h
 * @brief Used to change a links origin in environment
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
#ifndef TESSERACT_ENVIRONMENT_CHANGE_LINK_ORIGIN_COMMAND_H
#define TESSERACT_ENVIRONMENT_CHANGE_LINK_ORIGIN_COMMAND_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <memory>
#include <Eigen/Geometry>
#include <boost/serialization/export.hpp>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_environment/command.h>

namespace boost::serialization
{
class access;
}

namespace tesseract_environment
{
class ChangeLinkOriginCommand : public Command
{
public:
  // LCOV_EXCL_START
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  // LCOV_EXCL_STOP

  using Ptr = std::shared_ptr<ChangeLinkOriginCommand>;
  using ConstPtr = std::shared_ptr<const ChangeLinkOriginCommand>;

  ChangeLinkOriginCommand();

  ChangeLinkOriginCommand(std::string link_name, const Eigen::Isometry3d& origin);

  const std::string& getLinkName() const;
  const Eigen::Isometry3d& getOrigin() const;

  bool operator==(const ChangeLinkOriginCommand& rhs) const;
  bool operator!=(const ChangeLinkOriginCommand& rhs) const;

private:
  std::string link_name_;
  Eigen::Isometry3d origin_{ Eigen::Isometry3d::Identity() };

  friend class boost::serialization::access;
  friend struct tesseract_common::Serialization;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version);  // NOLINT
};
}  // namespace tesseract_environment

BOOST_CLASS_EXPORT_KEY(tesseract_environment::ChangeLinkOriginCommand)
#endif  // TESSERACT_ENVIRONMENT_CHANGE_LINK_ORIGIN_COMMAND_H
