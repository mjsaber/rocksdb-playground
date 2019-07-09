#pragma once

#include <string>
#include <vector>
#include <memory>

#include "services/http/data_source/DataSource.h"

namespace services {
namespace http {
namespace recommender {

class Recommender {
public:
  virtual std::vector<data_source::QueryResult> recommend(const std::string &key) = 0;
  
  void setDataSource(std::unique_ptr<data_source::DataSource> dataSource);

  virtual ~Recommender() {}
  
protected:
  data_source::DataSource* getDataSource();

private:
  std::unique_ptr<data_source::DataSource> dataSource_;
};
} // namespace recommender
} // namespace http
} // namespace services
