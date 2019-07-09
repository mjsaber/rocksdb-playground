#include "Recommender.h"

namespace services {
namespace http {
namespace recommender {
void Recommender::setDataSource(
    std::unique_ptr<data_source::DataSource> dataSource) {
  dataSource_ = std::move(dataSource);
}

data_source::DataSource *Recommender::getDataSource() {
  return dataSource_.get();
}
} // namespace recommender
} // namespace http
} // namespace services
