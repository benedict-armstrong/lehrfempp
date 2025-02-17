// Author: Benedict Armstrong (adapted from code by Huang Liaowang)
// Date: January 2020
// This is part of the LehrFEM++ code suite

/*
The ultimate goal is to have references to LaTeX documents in documentation
generated by Doxygen, and this script is to do the preprocessing before the
Doxygen parse the source file.
*/
#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/karma_string.hpp>
#include <boost/spirit/include/qi.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>

#include "filter.hpp"

struct Match {
  std::string Label;
  std::string title;
  std::string number;
  std::string page_number;
  std::string reference_name;
};

/*
This Filter replaces all the @lref{<label>} statements in a C++ files
with a label-specific string and the corresponding number from .aux file. Here
we assume the line corresponding to this label in .aux file looks like

\newlabel{<label>@cref}{{[<Label>][x][xxx]<number>}{[x][xx][x]<page_number>}}

We would like to replace @lref{<label>} with <Label> <number>, which means that
label is extracted from the first square brackets and the number comes from the
content after two brackets of <Label>.

If the tag is @lref_link{<label>}, then the output will be a hyperlink to the
lecture document with the label as the anchor or alternatively the page number
for parts of the document that are not labeled.

This code is heavily optimized for speed. In particular it will create a cache
file which contains a lookup-table of the .aux file so that we can map <label>
-> <Label> <number>
*/
class LectureDocRefFilter : public Filter {
 private:
  std::unordered_map<std::string, std::string> label_map_;
  std::vector<std::pair<std::string, std::string>> aux_table_;
  std::unordered_map<std::string, bool> link_with_page_num_;

  // Matches
  // \newlabel{<label>@cref}{{[<Label>][x][xxx]<number>}{[x][xx][x]<page_number>}}
  const std::regex label_pattern_1_ = std::regex(
      R"(\\newlabel\{(.*)@cref\}\{\{\[([^,\]]*)\](?:\[[^\}]*\])*([0-9a-zA-Z\.]*)\}(?:\{(?:\[[^\}]*\])([0-9\.]*)\})?\})",
      std::regex::optimize);

  // Matches
  // \newlabel{<label>}{{<number>}{<page_number>}{<title>}{<reference_name>}{}}
  const std::regex label_pattern_2_ = std::regex(
      R"(\\newlabel\{(.*)(?!@cref)\}\{\{(.*)\}\{(.*)\}\{(.*)\}\{(.*)\}\{(?:.*)\}\})",
      std::regex::optimize);

  const std::string lecture_doc_url_ =
      "https://www.sam.math.ethz.ch/~grsam/NUMPDEFL/NUMPDE.pdf";

  std::string aux_file_;
  std::string file_;

 public:
  explicit LectureDocRefFilter(std::string file, std::string aux_file);

  ~LectureDocRefFilter() override = default;

  /**
   * @brief Reads a file from disk into a std::string. If the file doesn't
   * exist or cannot be opened, an empty optional is returned.
   * @param file_name The path to the file that should be opened.
   */
  std::optional<std::string> ReadFile(const std::string& file_name);

  /**
   * @brief Load the .aux file and create the `aux_table_`
   *
   * The line in .aux file we are interested is assumed to look like:
   * `\newlabel{<label>@cref}{{[<Label>][x][xxx]<number>}{[x][xx][x]xxx}}`
   *
   *
   * If there is a cache file present, the hash of the `.aux` file is compared
   * to the stored hash in the cache file. If they agree, the lookup table is
   * loaded from cache. Otherwise the lookup table (`aux_table_`) is created
   * from the `.aux` file.
   */
  void LoadAuxTable();
  /**
   * @brief Goes through the source file passed by doxygen (second commandline
   * argument) and replace @lref{} with the corresponding value in the lookup
   * table (`aux_table_`). The result is output to the console (`std::cout`)
   */
  std::string filter(const std::string& input) override;
};
