/**
 * @file contact_managers_plugin_factory.h
 * @brief Factory for loading contact managers as plugins
 *
 * @author Levi Armstrong
 * @date October 25, 2021
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
#ifndef TESSERACT_COLLISION_CONTACT_MANAGERS_PLUGIN_FACTORY_H
#define TESSERACT_COLLISION_CONTACT_MANAGERS_PLUGIN_FACTORY_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <string>
#include <memory>
#include <map>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_common/fwd.h>
#include <tesseract_common/plugin_info.h>
#include <boost_plugin_loader/plugin_loader.h>
#include <boost_plugin_loader/macros.h>
#include <filesystem>

// clang-format off
#define TESSERACT_ADD_DISCRETE_MANAGER_PLUGIN(DERIVED_CLASS, ALIAS)                                                    \
  EXPORT_CLASS_SECTIONED(DERIVED_CLASS, ALIAS, DiscColl)

#define TESSERACT_ADD_CONTINUOUS_MANAGER_PLUGIN(DERIVED_CLASS, ALIAS)                                                  \
  EXPORT_CLASS_SECTIONED(DERIVED_CLASS, ALIAS, ContColl)
// clang-format on

namespace tesseract_collision
{
/** @brief Forward declare Plugin Factory */
class ContactManagersPluginFactory;
class DiscreteContactManager;
class ContinuousContactManager;

/** @brief Define a discrete contact manager plugin which the factory can create an instance */
class DiscreteContactManagerFactory
{
public:
  using Ptr = std::shared_ptr<DiscreteContactManagerFactory>;
  using ConstPtr = std::shared_ptr<const DiscreteContactManagerFactory>;

  virtual ~DiscreteContactManagerFactory() = default;

  /**
   * @brief Create Discrete Contact Manager Object
   * @param name The name of the contact manager object
   * @return If failed to create, nullptr is returned.
   */
  virtual std::unique_ptr<DiscreteContactManager> create(const std::string& name, const YAML::Node& config) const = 0;

protected:
  static std::string getSection();
  friend class boost_plugin_loader::PluginLoader;
};

/** @brief Define a continuous contact manager plugin which the factory can create an instance */
class ContinuousContactManagerFactory
{
public:
  using Ptr = std::shared_ptr<ContinuousContactManagerFactory>;
  using ConstPtr = std::shared_ptr<const ContinuousContactManagerFactory>;

  virtual ~ContinuousContactManagerFactory() = default;

  /**
   * @brief Create Inverse Kinematics Object
   * @param name The name of the contact manager object
   * @return If failed to create, nullptr is returned.
   */
  virtual std::unique_ptr<ContinuousContactManager> create(const std::string& solver_name,
                                                           const YAML::Node& config) const = 0;

protected:
  static std::string getSection();
  friend class boost_plugin_loader::PluginLoader;
};

class ContactManagersPluginFactory
{
public:
  ContactManagersPluginFactory();
  ~ContactManagersPluginFactory();
  ContactManagersPluginFactory(const ContactManagersPluginFactory&) = default;
  ContactManagersPluginFactory& operator=(const ContactManagersPluginFactory&) = default;
  ContactManagersPluginFactory(ContactManagersPluginFactory&&) = default;
  ContactManagersPluginFactory& operator=(ContactManagersPluginFactory&&) = default;

  /**
   * @brief Load plugins from yaml node
   * @param config The config node
   */
  ContactManagersPluginFactory(YAML::Node config, const tesseract_common::ResourceLocator& locator);

  /**
   * @brief Load plugins from file path
   * @param config The config file path
   */
  ContactManagersPluginFactory(const std::filesystem::path& config, const tesseract_common::ResourceLocator& locator);

  /**
   * @brief Load plugins from string
   * @param config The config string
   */
  ContactManagersPluginFactory(const std::string& config, const tesseract_common::ResourceLocator& locator);

  /**
   * @brief Add location for the plugin loader to search
   * @param path The full path to the directory
   */
  void addSearchPath(const std::string& path);

  /**
   * @brief Get the plugin search paths
   * @return The search paths
   */
  std::set<std::string> getSearchPaths() const;

  /**
   * @brief Clear the search paths
   *
   */
  void clearSearchPaths();

  /**
   * @brief Add a library to search for plugin name
   * @param library_name The library name without the prefix or suffix
   */
  void addSearchLibrary(const std::string& library_name);

  /**
   * @brief Get the plugin search libraries
   * @return The search libraries
   */
  std::set<std::string> getSearchLibraries() const;

  /**
   * @brief Clean the search libraries
   *
   */
  void clearSearchLibraries();

  /**
   * @brief Add a discrete contact manager plugin
   * @param name The name
   * @param plugin_info The plugin information
   */
  void addDiscreteContactManagerPlugin(const std::string& name, tesseract_common::PluginInfo plugin_info);

  /**
   * @brief Check if it has discrete contact manager plugins
   * @return True if discrete PluginInfoMap is not empty, otherwise fale
   */
  bool hasDiscreteContactManagerPlugins() const;

  /**
   * @brief Get the map of discrete contact manager plugin
   * @return A map of plugins
   */
  tesseract_common::PluginInfoMap getDiscreteContactManagerPlugins() const;

  /**
   * @brief Remove discrete contact manager plugin
   * @param name The name of the contact manager to remove
   */
  void removeDiscreteContactManagerPlugin(const std::string& name);

  /**
   * @brief Set a default discrete contact manager
   * @param name The name
   */
  void setDefaultDiscreteContactManagerPlugin(const std::string& name);

  /**
   * @brief Get the default discrete contact manager
   * @return The default discrete contact manager
   */
  std::string getDefaultDiscreteContactManagerPlugin() const;

  /**
   * @brief Add a continuous contact manager plugin
   * @param name The name
   * @param plugin_info The plugin information
   */
  void addContinuousContactManagerPlugin(const std::string& name, tesseract_common::PluginInfo plugin_info);

  /**
   * @brief Check if it has continuous contact manager plugins
   * @return True if continuous PluginInfoMap is not empty, otherwise fale
   */
  bool hasContinuousContactManagerPlugins() const;

  /**
   * @brief Get the map of continuous contact manager plugin
   * @return A map of plugins
   */
  tesseract_common::PluginInfoMap getContinuousContactManagerPlugins() const;

  /**
   * @brief Remove continuous contact manager plugin
   * @param name The name of the contact manager to remove
   */
  void removeContinuousContactManagerPlugin(const std::string& name);

  /**
   * @brief Set a default continuous contact manager
   * @param name The name
   */
  void setDefaultContinuousContactManagerPlugin(const std::string& name);

  /**
   * @brief Get the default continuous contact manager
   * @return The default continuous contact manager name
   */
  std::string getDefaultContinuousContactManagerPlugin() const;

  /**
   * @brief Get discrete contact manager object given name
   * @details This looks for discrete contact manager plugin info. If not found nullptr is returned.
   * @param name The name
   */
  std::unique_ptr<DiscreteContactManager> createDiscreteContactManager(const std::string& name) const;

  /**
   * @brief Get discrete contact manager object given plugin info
   * @param name The name
   * @param plugin_info The plugin information to create kinematics object
   */
  std::unique_ptr<DiscreteContactManager>
  createDiscreteContactManager(const std::string& name, const tesseract_common::PluginInfo& plugin_info) const;

  /**
   * @brief Get continuous contact manager object given name
   * @details This looks for continuous contact manager plugin info. If not found nullptr is returned.
   * @param name The name
   */
  std::unique_ptr<ContinuousContactManager> createContinuousContactManager(const std::string& name) const;

  /**
   * @brief Get continuous contact manager object given plugin info
   * @param name The name
   * @param plugin_info The plugin information to create kinematics object
   */
  std::unique_ptr<ContinuousContactManager>
  createContinuousContactManager(const std::string& name, const tesseract_common::PluginInfo& plugin_info) const;

  /**
   * @brief Save the plugin information to a yaml config file
   * @param file_path The file path
   */
  void saveConfig(const std::filesystem::path& file_path) const;

  /**
   * @brief Get the plugin information config as a yaml node
   * @return The plugin information config yaml node/
   */
  YAML::Node getConfig() const;

private:
  mutable std::map<std::string, DiscreteContactManagerFactory::Ptr> discrete_factories_;
  mutable std::map<std::string, ContinuousContactManagerFactory::Ptr> continuous_factories_;
  tesseract_common::PluginInfoContainer discrete_plugin_info_;
  tesseract_common::PluginInfoContainer continuous_plugin_info_;
  boost_plugin_loader::PluginLoader plugin_loader_;

  void loadConfig(const YAML::Node& config);
};
}  // namespace tesseract_collision
#endif  // TESSERACT_COLLISION_CONTACT_MANAGERS_PLUGIN_FACTORY_H
