/**
 * @file mesh.h
 * @brief Parse mesh from xml string
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
#ifndef TESSERACT_URDF_MESH_H
#define TESSERACT_URDF_MESH_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <memory>
#include <vector>
#include <string_view>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_common/fwd.h>
#include <tesseract_geometry/fwd.h>

namespace tinyxml2
{
class XMLElement;  // NOLINT
class XMLDocument;
}  // namespace tinyxml2

namespace tesseract_urdf
{
static constexpr std::string_view MESH_ELEMENT_NAME = "mesh";

/**
 * @brief Parse xml element mesh
 * @param xml_element The xml element
 * @param locator The Tesseract resource locator
 * @param visual Indicate if visual
 * @param make_convex Flag to indicate if the mesh should be converted to a convex hull
 * @return A vector of Tesseract Meshes
 */
std::vector<std::shared_ptr<tesseract_geometry::PolygonMesh>>
parseMesh(const tinyxml2::XMLElement* xml_element,
          const tesseract_common::ResourceLocator& locator,
          bool visual,
          bool make_convex);

/**
 * @brief writeMesh Write a mesh to URDF XML and PLY file
 * @param mesh Mesh to be saved out and described in XML
 * @param doc XML Document to manage generated XML
 * @param package_path /<path>/<to>/<your-package>.  If set, geometry will be saved relative to the package.  If not
 * set, geometry will be saved with absolute paths.
 * @param filename Desired file location.  If package_path is set, this should be relative to the package, If not, this
 * should be an absolute path
 * @return XML element representing the mesh object in URDF format.
 */
tinyxml2::XMLElement* writeMesh(const std::shared_ptr<const tesseract_geometry::PolygonMesh>& mesh,
                                tinyxml2::XMLDocument& doc,
                                const std::string& package_path,
                                const std::string& filename);

}  // namespace tesseract_urdf

#endif  // TESSERACT_URDF_MESH_H
