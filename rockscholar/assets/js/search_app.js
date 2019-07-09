
class ResultEntry extends React.Component {
  render() {
    return (
      <div class="border-bottom result-entry">
        <strong class="d-block text-gray-dark">{this.props.result.title}</strong>
        <p class="small">{this.props.result.citation}</p>
      </div>
    );
  }
}

class ResultList extends React.Component {
  render() {
   return (
     <div class="col">
       <h6 class="border-bottom result-entry">Search Results</h6>
       {this.props.results.map(result => (
         <ResultEntry result={result} />
       ))}
     </div>
   );
  }
}

class SearchApp extends React.Component {
  constructor(props) {
    super(props);
    this.state = { search_key: "", results: []};
    this.handleChange = this.handleChange.bind(this);
    this.handleResults = this.handleResults.bind(this);
  }

  handleChange(e) {
    this.setState({ search_key: e.target.value });
    var dataJson = { search_key: e.target.value };
    $.ajax({
      type: "POST",
      url: "/api/search",
      data: JSON.stringify(dataJson),
      dataType: "json",
      success: this.handleResults
    });
  }

  handleResults(data) {
    this.setState(prevState => ({
      search_key: prevState.search_key,
      results: data
    }));
  }

  render() {
    return (
      <div className="SearchApp" class="container">
        <div class="row bg-purple rounded">
          <div class="col-2 logo-block">
            <img class="fit-logo col" src="logo.png"/>
          </div>
          <div class="col">
            <div class="search-label-group">
              <input
                class="col"
                type="text"
                onChange={this.handleChange}
                value={this.state.search_key}
                id="inputSearchKey"
                placeholder="Search key"
                autofocus
              />
              <label for="inputSearchKey">Search key</label>
            </div>
          </div>
        </div>

        <div class="row bg-white rounded box-shadow result-pannel">
          <ResultList
            class="col"
            results={this.state.results}
          />
        </div>
      </div>
    );
  }
}

