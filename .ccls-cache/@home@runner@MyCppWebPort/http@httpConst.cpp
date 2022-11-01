#include "httpConst.hpp"

std::map<std::string, std::string> http::content_types;

void http::register_content_type() {
  // Applications
  http::content_types["java-archive"] = "application/java-archive";
  //http::content_types["css"] = "application/EDI-X12";
  //http::content_types["css"] = "application/EDIFACT";
  http::content_types["javascript"] = "application/javascript";
  http::content_types["stream"] = "application/octet-stream";
  http::content_types["ogg"] = "application/ogg";
  http::content_types["pdf"] = "application/pdf";
  http::content_types["xhtml+xml"] = "application/xhtml+xml";
  //http::content_types["css"] = "application/x-shockwave-flash";
  http::content_types["json"] = "application/json";
  http::content_types["ld+json"] = "application/ld+json";
  http::content_types["xml"] = "application/xml";
  http::content_types["zip"] = "application/zip";
  http::content_types["x-wwww-form-urlencoded"] = "application/x-www-form-urlencoded";

  //http::content_types["css"] = "application/vnd.android.package-archive";
  //http::content_types["css"] = "application/vnd.oasis.opendocument.text";
  //http::content_types["css"] = "application/vnd.oasis.opendocument.spreadsheet";
  //http::content_types["css"] = "application/vnd.oasis.opendocument.presentation";
  //http::content_types["css"] = "application/vnd.oasis.opendocument.graphics";
  //http::content_types["css"] = "application/vnd.ms-excel ";
  //http::content_types["css"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
  //http::content_types["css"] = "application/vnd.ms-powerpoint";
  //http::content_types["css"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
  //http::content_types["css"] = "application/msword";
  //http::content_types["css"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
  //http::content_types["css"] = "application/vnd.mozilla.xul+xml";

  // Audio
  http::content_types["mpeg"] = "audio/mpeg";
  //http::content_types["css"] = "audio/x-ms-wma";
  //http::content_types["css"] = "audio/vnd.rn-realaudio";
  //http::content_types["css"] = "audio/x-wav";

  // Image
  http::content_types["gif"] = "image/gif";
  http::content_types["jpeg"] = "image/jpeg";
  http::content_types["png"] = "image/png";
  http::content_types["tiff"] = "image/tiff";
  http::content_types["vnd.microsoft.icon"] = "image/vnd.microsoft.icon";
  http::content_types["x-icon"] = "image/x-icon";
  //http::content_types["css"] = "image/vnd.djvu";
  http::content_types["svg+xml"] = "image/svg+xml";

  //http::content_types["css"] = "multipart/mixed";
  //http::content_types["css"] = "multipart/alternative";
  //http::content_types["css"] = "multipart/related";
  http::content_types["form-data"] = "multipart/form-data";

  http::content_types["css"] = "text/css";
  http::content_types["csv"] = "text/csv";
  http::content_types["html"] = "text/html";
  //http::content_types["css"] = "text/javascript"; // obsolete
  http::content_types["plain"] = "text/plain";
  http::content_types["xml"] = "text/xml";

  http::content_types["mpeg"] = "video/mpeg";
  http::content_types["mp4"] = "video/mp4";
  //http::content_types["css"] = "video/quicktime";
  //http::content_types["css"] = "video/x-ms-wmv";
  //http::content_types["css"] = "video/x-msvideo";
  //http::content_types["css"] = "video/x-flv";
  //http::content_types["css"] = "video/webm";
}

std::string http::get_content_type(std::string content) {
  auto content_type = http::content_types.find(content);
  if (content_type == http::content_types.end())
    return "";
  return content_type->second;
}

std::string http::get_name_of_content(std::string content_type) {
  for (const auto &pair : http::content_types) {
    if (pair.second == content_type)
      return pair.first;
  }
  return "";
}