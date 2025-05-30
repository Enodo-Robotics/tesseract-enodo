#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <gtest/gtest.h>
#include <Eigen/Geometry>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_urdf/sdf_mesh.h>
#include <tesseract_geometry/impl/sdf_mesh.h>
#include <tesseract_common/resource_locator.h>
#include "tesseract_urdf_common_unit.h"

static std::string getTempPkgPath()
{
  std::string tmp = tesseract_common::getTempPath();
  std::string tmppkg = tmp + "tmppkg";
  if (!std::filesystem::is_directory(tmppkg) || !std::filesystem::exists(tmppkg))
  {
    std::filesystem::create_directory(tmppkg);
  }
  return tmppkg;
}

TEST(TesseractURDFUnit, parse_sdf_mesh)  // NOLINT
{
  tesseract_common::GeneralResourceLocator resource_locator;
  {
    std::string str =
        R"(<tesseract:sdf_mesh filename="package://tesseract_support/meshes/sphere_p25m.stl" scale="1 2 1" extra="0 0 0"/>)";
    std::vector<tesseract_geometry::SDFMesh::Ptr> geom;
    EXPECT_TRUE(runTest<std::vector<tesseract_geometry::SDFMesh::Ptr>>(geom,
                                                                       &tesseract_urdf::parseSDFMesh,
                                                                       str,
                                                                       tesseract_urdf::SDF_MESH_ELEMENT_NAME.data(),
                                                                       resource_locator,
                                                                       true));
    EXPECT_TRUE(geom.size() == 1);
    EXPECT_TRUE(geom[0]->getFaceCount() == 80);
    EXPECT_TRUE(geom[0]->getVertexCount() == 240);
    EXPECT_NEAR(geom[0]->getScale()[0], 1, 1e-5);
    EXPECT_NEAR(geom[0]->getScale()[1], 2, 1e-5);
    EXPECT_NEAR(geom[0]->getScale()[2], 1, 1e-5);
  }

  {
    std::string str = R"(<tesseract:sdf_mesh filename="package://tesseract_support/meshes/sphere_p25m.stl"/>)";
    std::vector<tesseract_geometry::SDFMesh::Ptr> geom;
    EXPECT_TRUE(runTest<std::vector<tesseract_geometry::SDFMesh::Ptr>>(geom,
                                                                       &tesseract_urdf::parseSDFMesh,
                                                                       str,
                                                                       tesseract_urdf::SDF_MESH_ELEMENT_NAME.data(),
                                                                       resource_locator,
                                                                       true));
    EXPECT_TRUE(geom.size() == 1);
    EXPECT_TRUE(geom[0]->getFaceCount() == 80);
    EXPECT_TRUE(geom[0]->getVertexCount() == 240);
    EXPECT_NEAR(geom[0]->getScale()[0], 1, 1e-5);
    EXPECT_NEAR(geom[0]->getScale()[1], 1, 1e-5);
    EXPECT_NEAR(geom[0]->getScale()[2], 1, 1e-5);
  }

  {
    std::string str = R"(<tesseract:sdf_mesh filename="package://tesseract_support/meshes/sphere_p25m.stl"/>)";
    std::vector<tesseract_geometry::SDFMesh::Ptr> geom;
    EXPECT_TRUE(runTest<std::vector<tesseract_geometry::SDFMesh::Ptr>>(geom,
                                                                       &tesseract_urdf::parseSDFMesh,
                                                                       str,
                                                                       tesseract_urdf::SDF_MESH_ELEMENT_NAME.data(),
                                                                       resource_locator,
                                                                       false));
    EXPECT_TRUE(geom.size() == 1);
    EXPECT_TRUE(geom[0]->getFaceCount() == 80);
    EXPECT_TRUE(geom[0]->getVertexCount() == 42);
    EXPECT_NEAR(geom[0]->getScale()[0], 1, 1e-5);
    EXPECT_NEAR(geom[0]->getScale()[1], 1, 1e-5);
    EXPECT_NEAR(geom[0]->getScale()[2], 1, 1e-5);
  }

  {
    std::string str = R"(<tesseract:sdf_mesh filename="abc" scale="1 2 1"/>)";
    std::vector<tesseract_geometry::SDFMesh::Ptr> geom;
    EXPECT_FALSE(runTest<std::vector<tesseract_geometry::SDFMesh::Ptr>>(geom,
                                                                        &tesseract_urdf::parseSDFMesh,
                                                                        str,
                                                                        tesseract_urdf::SDF_MESH_ELEMENT_NAME.data(),
                                                                        resource_locator,
                                                                        true));
  }

  {
    std::string str =
        R"(<tesseract:sdf_mesh filename="package://tesseract_support/meshes/sphere_p25m.stl" scale="1 a 1"/>)";
    std::vector<tesseract_geometry::SDFMesh::Ptr> geom;
    EXPECT_FALSE(runTest<std::vector<tesseract_geometry::SDFMesh::Ptr>>(geom,
                                                                        &tesseract_urdf::parseSDFMesh,
                                                                        str,
                                                                        tesseract_urdf::SDF_MESH_ELEMENT_NAME.data(),
                                                                        resource_locator,
                                                                        true));
  }

  {
    std::string str =
        R"(<tesseract:sdf_mesh filename="package://tesseract_support/meshes/sphere_p25m.stl" scale="a 1 1"/>)";
    std::vector<tesseract_geometry::SDFMesh::Ptr> geom;
    EXPECT_FALSE(runTest<std::vector<tesseract_geometry::SDFMesh::Ptr>>(geom,
                                                                        &tesseract_urdf::parseSDFMesh,
                                                                        str,
                                                                        tesseract_urdf::SDF_MESH_ELEMENT_NAME.data(),
                                                                        resource_locator,
                                                                        true));
  }

  {
    std::string str =
        R"(<tesseract:sdf_mesh filename="package://tesseract_support/meshes/sphere_p25m.stl" scale="1 1 a"/>)";
    std::vector<tesseract_geometry::SDFMesh::Ptr> geom;
    EXPECT_FALSE(runTest<std::vector<tesseract_geometry::SDFMesh::Ptr>>(geom,
                                                                        &tesseract_urdf::parseSDFMesh,
                                                                        str,
                                                                        tesseract_urdf::SDF_MESH_ELEMENT_NAME.data(),
                                                                        resource_locator,
                                                                        true));
  }

  {
    std::string str =
        R"(<tesseract:sdf_mesh filename="package://tesseract_support/meshes/sphere_p25m.ply" scale="-1 2 1" />)";
    std::vector<tesseract_geometry::SDFMesh::Ptr> geom;
    EXPECT_FALSE(runTest<std::vector<tesseract_geometry::SDFMesh::Ptr>>(geom,
                                                                        &tesseract_urdf::parseSDFMesh,
                                                                        str,
                                                                        tesseract_urdf::SDF_MESH_ELEMENT_NAME.data(),
                                                                        resource_locator,
                                                                        true));
    EXPECT_TRUE(geom.empty());
  }

  {
    std::string str =
        R"(<tesseract:sdf_mesh filename="package://tesseract_support/meshes/sphere_p25m.ply" scale="1 -1 1" />)";
    std::vector<tesseract_geometry::SDFMesh::Ptr> geom;
    EXPECT_FALSE(runTest<std::vector<tesseract_geometry::SDFMesh::Ptr>>(geom,
                                                                        &tesseract_urdf::parseSDFMesh,
                                                                        str,
                                                                        tesseract_urdf::SDF_MESH_ELEMENT_NAME.data(),
                                                                        resource_locator,
                                                                        false));
    EXPECT_TRUE(geom.empty());
  }

  {
    std::string str =
        R"(<tesseract:sdf_mesh filename="package://tesseract_support/meshes/sphere_p25m.ply" scale="1 2 -1" />)";
    std::vector<tesseract_geometry::SDFMesh::Ptr> geom;
    EXPECT_FALSE(runTest<std::vector<tesseract_geometry::SDFMesh::Ptr>>(geom,
                                                                        &tesseract_urdf::parseSDFMesh,
                                                                        str,
                                                                        tesseract_urdf::SDF_MESH_ELEMENT_NAME.data(),
                                                                        resource_locator,
                                                                        false));
    EXPECT_TRUE(geom.empty());
  }

  {
    std::string str =
        R"(<tesseract:sdf_mesh filename="package://tesseract_support/meshes/sphere_p25m.stl" scale="1 2 1 3"/>)";
    std::vector<tesseract_geometry::SDFMesh::Ptr> geom;
    EXPECT_FALSE(runTest<std::vector<tesseract_geometry::SDFMesh::Ptr>>(geom,
                                                                        &tesseract_urdf::parseSDFMesh,
                                                                        str,
                                                                        tesseract_urdf::SDF_MESH_ELEMENT_NAME.data(),
                                                                        resource_locator,
                                                                        true));
  }

  {
    std::string str = R"(<tesseract:sdf_mesh scale="1 2 1"/>)";
    std::vector<tesseract_geometry::SDFMesh::Ptr> geom;
    EXPECT_FALSE(runTest<std::vector<tesseract_geometry::SDFMesh::Ptr>>(geom,
                                                                        &tesseract_urdf::parseSDFMesh,
                                                                        str,
                                                                        tesseract_urdf::SDF_MESH_ELEMENT_NAME.data(),
                                                                        resource_locator,
                                                                        true));
  }

  {
    std::string str = R"(<tesseract:sdf_mesh />)";
    std::vector<tesseract_geometry::SDFMesh::Ptr> geom;
    EXPECT_FALSE(runTest<std::vector<tesseract_geometry::SDFMesh::Ptr>>(geom,
                                                                        &tesseract_urdf::parseSDFMesh,
                                                                        str,
                                                                        tesseract_urdf::SDF_MESH_ELEMENT_NAME.data(),
                                                                        resource_locator,
                                                                        true));
  }
}

TEST(TesseractURDFUnit, write_sdf_mesh)  // NOLINT
{
  {
    tesseract_common::VectorVector3d vertices = { Eigen::Vector3d(0, 0, 0),
                                                  Eigen::Vector3d(1, 0, 0),
                                                  Eigen::Vector3d(0, 1, 0) };
    Eigen::VectorXi indices(4);
    indices << 3, 0, 1, 2;
    tesseract_geometry::SDFMesh::Ptr sdf_mesh = std::make_shared<tesseract_geometry::SDFMesh>(
        std::make_shared<tesseract_common::VectorVector3d>(vertices), std::make_shared<Eigen::VectorXi>(indices));
    std::string text;
    EXPECT_EQ(0,
              writeTest<tesseract_geometry::SDFMesh::Ptr>(
                  sdf_mesh, &tesseract_urdf::writeSDFMesh, text, getTempPkgPath(), std::string("sdf0.ply")));
    EXPECT_EQ(text, R"(<tesseract:sdf_mesh filename="package://tmppkg/sdf0.ply"/>)");
  }

  {  // With scale
    tesseract_common::VectorVector3d vertices = { Eigen::Vector3d(0, 0, 0),
                                                  Eigen::Vector3d(1, 0, 0),
                                                  Eigen::Vector3d(0, 1, 0) };

    Eigen::VectorXi indices(4);
    indices << 3, 0, 1, 2;
    Eigen::Vector3d scale(0.5, 0.5, 0.5);
    tesseract_geometry::SDFMesh::Ptr sdf_mesh =
        std::make_shared<tesseract_geometry::SDFMesh>(std::make_shared<tesseract_common::VectorVector3d>(vertices),
                                                      std::make_shared<Eigen::VectorXi>(indices),
                                                      nullptr,
                                                      scale);
    std::string text;
    EXPECT_EQ(0,
              writeTest<tesseract_geometry::SDFMesh::Ptr>(
                  sdf_mesh, &tesseract_urdf::writeSDFMesh, text, getTempPkgPath(), std::string("sdf1.ply")));
    EXPECT_EQ(text, R"(<tesseract:sdf_mesh filename="package://tmppkg/sdf1.ply" scale="0.5 0.5 0.5"/>)");
  }

  {
    tesseract_geometry::SDFMesh::Ptr sdf_mesh = nullptr;
    std::string text;
    EXPECT_EQ(
        1,
        writeTest<tesseract_geometry::SDFMesh::Ptr>(
            sdf_mesh, &tesseract_urdf::writeSDFMesh, text, tesseract_common::getTempPath(), std::string("sdf2.ply")));
    EXPECT_EQ(text, "");
  }
}
