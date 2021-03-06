#include "common/config/runtime_utility.h"

namespace Envoy {
namespace Config {

void translateRuntime(const envoy::config::bootstrap::v2::Runtime& runtime_config,
                      envoy::config::bootstrap::v2::LayeredRuntime& layered_runtime_config) {
  layered_runtime_config.add_layers()->mutable_static_layer()->MergeFrom(runtime_config.base());
  if (!runtime_config.symlink_root().empty()) {
    {
      auto* layer = layered_runtime_config.add_layers();
      layer->set_name("root");
      layer->mutable_disk_layer()->set_symlink_root(runtime_config.symlink_root() + "/" +
                                                    runtime_config.subdirectory());
    }
    if (!runtime_config.override_subdirectory().empty()) {
      auto* layer = layered_runtime_config.add_layers();
      layer->set_name("override");
      layer->mutable_disk_layer()->set_symlink_root(runtime_config.symlink_root() + "/" +
                                                    runtime_config.override_subdirectory());
      layer->mutable_disk_layer()->set_append_service_cluster(true);
    }
  }
  layered_runtime_config.add_layers()->mutable_admin_layer();
}

} // namespace Config
} // namespace Envoy
