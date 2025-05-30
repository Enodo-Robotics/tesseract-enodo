/**
 * @file box.cpp
 * @brief Parse box from xml string
 *
 * @author Levi Armstrong
 * @date September 1, 2019
 * @version TODO
 * @bug No known bugs
 *
 * @copyright Copyright (c) 2019, Southwest Research Institute
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
#include <stdexcept>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <Eigen/Geometry>
#include <tesseract_common/utils.h>
#include <tinyxml2.h>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_geometry/impl/sdf_mesh.h>
#include <tesseract_geometry/mesh_parser.h>
#include <tesseract_urdf/sdf_mesh.h>
#include <tesseract_common/resource_locator.h>
#include <tesseract_urdf/utils.h>

namespace tesseract_urdf
{
std::vector<tesseract_geometry::SDFMesh::Ptr> parseSDFMesh(const tinyxml2::XMLElement* xml_element,
                                                           const tesseract_common::ResourceLocator& locator,
                                                           bool visual)
{
  std::vector<tesseract_geometry::SDFMesh::Ptr> meshes;

  std::string filename;
  if (tesseract_common::QueryStringAttribute(xml_element, "filename", filename) != tinyxml2::XML_SUCCESS)
    std::throw_with_nested(std::runtime_error("SDFMesh: Missing or failed parsing attribute 'filename'!"));

  std::string scale_string;
  Eigen::Vector3d scale(1, 1, 1);
  if (tesseract_common::QueryStringAttribute(xml_element, "scale", scale_string) == tinyxml2::XML_SUCCESS)
  {
    std::vector<std::string> tokens;
    boost::split(tokens, scale_string, boost::is_any_of(" "), boost::token_compress_on);
    if (tokens.size() != 3 || !tesseract_common::isNumeric(tokens))
      std::throw_with_nested(std::runtime_error("SDFMesh: Failed parsing attribute 'scale'!"));

    double sx{ 0 }, sy{ 0 }, sz{ 0 };
    // No need to check return values because the tokens are verified above
    tesseract_common::toNumeric<double>(tokens[0], sx);
    tesseract_common::toNumeric<double>(tokens[1], sy);
    tesseract_common::toNumeric<double>(tokens[2], sz);

    if (!(sx > 0))
      std::throw_with_nested(std::runtime_error("SDFMesh: Scale x is not greater than zero!"));

    if (!(sy > 0))
      std::throw_with_nested(std::runtime_error("SDFMesh: Scale y is not greater than zero!"));

    if (!(sz > 0))
      std::throw_with_nested(std::runtime_error("SDFMesh: Scale z is not greater than zero!"));

    scale = Eigen::Vector3d(sx, sy, sz);
  }

  if (visual)
    meshes = tesseract_geometry::createMeshFromResource<tesseract_geometry::SDFMesh>(
        locator.locateResource(filename), scale, true, true, true, true, true);
  else
    meshes = tesseract_geometry::createMeshFromResource<tesseract_geometry::SDFMesh>(
        locator.locateResource(filename), scale, true, false);

  if (meshes.empty())
    std::throw_with_nested(std::runtime_error("SDFMesh: Error importing meshes from filename: '" + filename + "'!"));

  return meshes;
}

tinyxml2::XMLElement* writeSDFMesh(const std::shared_ptr<const tesseract_geometry::SDFMesh>& sdf_mesh,
                                   tinyxml2::XMLDocument& doc,
                                   const std::string& package_path,
                                   const std::string& filename)
{
  if (sdf_mesh == nullptr)
    std::throw_with_nested(std::runtime_error("SDF Mesh is nullptr and cannot be converted to XML"));
  tinyxml2::XMLElement* xml_element = doc.NewElement(SDF_MESH_ELEMENT_NAME.data());
  Eigen::IOFormat eigen_format(Eigen::StreamPrecision, Eigen::DontAlignCols, " ", " ");

  try
  {
    writeMeshToFile(sdf_mesh, trailingSlash(package_path) + noLeadingSlash(filename));
  }
  catch (...)
  {
    std::throw_with_nested(std::runtime_error("Failed to write convex mesh to file: " + package_path + filename));
  }
  xml_element->SetAttribute("filename", makeURDFFilePath(package_path, filename).c_str());

  if (!sdf_mesh->getScale().isOnes())
  {
    std::stringstream scale_string;
    scale_string << sdf_mesh->getScale().format(eigen_format);
    xml_element->SetAttribute("scale", scale_string.str().c_str());
  }
  return xml_element;
}

}  // namespace tesseract_urdf
