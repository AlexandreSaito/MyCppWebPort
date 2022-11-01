#include "content.hpp"

std::map<std::string, std::string> web::http::content_types;

void web::http::register_content_type() {
  // Applications
  content_types["java-archive"] = "application/java-archive";
  // content_types["css"] = "application/EDI-X12";
  // content_types["css"] = "application/EDIFACT";
  content_types["javascript"] = "application/javascript";
  content_types["stream"] = "application/octet-stream";
  content_types["ogg"] = "application/ogg";
  content_types["pdf"] = "application/pdf";
  content_types["xhtml+xml"] = "application/xhtml+xml";
  // content_types["css"] = "application/x-shockwave-flash";
  content_types["json"] = "application/json";
  content_types["ld+json"] = "application/ld+json";
  content_types["xml"] = "application/xml";
  content_types["zip"] = "application/zip";
  content_types["x-wwww-form-urlencoded"] = "application/x-www-form-urlencoded";

  // content_types["css"] = "application/vnd.android.package-archive";
  // content_types["css"] = "application/vnd.oasis.opendocument.text";
  // content_types["css"] = "application/vnd.oasis.opendocument.spreadsheet";
  // content_types["css"] = "application/vnd.oasis.opendocument.presentation";
  // content_types["css"] = "application/vnd.oasis.opendocument.graphics";
  // content_types["css"] = "application/vnd.ms-excel ";
  // content_types["css"] =
  // "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
  // content_types["css"] = "application/vnd.ms-powerpoint";
  // content_types["css"] =
  // "application/vnd.openxmlformats-officedocument.presentationml.presentation";
  // content_types["css"] = "application/msword";
  // content_types["css"] =
  // "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
  // content_types["css"] = "application/vnd.mozilla.xul+xml";

  // Audio
  content_types["mpeg"] = "audio/mpeg";
  // content_types["css"] = "audio/x-ms-wma";
  // content_types["css"] = "audio/vnd.rn-realaudio";
  // content_types["css"] = "audio/x-wav";

  // Image
  content_types["gif"] = "image/gif";
  content_types["jpeg"] = "image/jpeg";
  content_types["png"] = "image/png";
  content_types["tiff"] = "image/tiff";
  content_types["vnd.microsoft.icon"] = "image/vnd.microsoft.icon";
  content_types["x-icon"] = "image/x-icon";
  // content_types["css"] = "image/vnd.djvu";
  content_types["svg+xml"] = "image/svg+xml";

  // content_types["css"] = "multipart/mixed";
  // content_types["css"] = "multipart/alternative";
  // content_types["css"] = "multipart/related";
  content_types["form-data"] = "multipart/form-data";

  content_types["css"] = "text/css";
  content_types["csv"] = "text/csv";
  content_types["html"] = "text/html";
  // content_types["css"] = "text/javascript"; // obsolete
  content_types["plain"] = "text/plain";
  content_types["xml"] = "text/xml";

  content_types["mpeg"] = "video/mpeg";
  content_types["mp4"] = "video/mp4";
  // content_types["css"] = "video/quicktime";
  // content_types["css"] = "video/x-ms-wmv";
  // content_types["css"] = "video/x-msvideo";
  // content_types["css"] = "video/x-flv";
  // content_types["css"] = "video/webm";
}

const std::string web::http::get_content_type(std::string content) {
  auto content_type = http::content_types.find(content);
  if (content_type == http::content_types.end())
    return "";
  return content_type->second;
}

const std::string web::http::get_name_of_content(std::string content_type) {
  for (const auto &pair : http::content_types) {
    if (pair.second == content_type)
      return pair.first;
  }
  return "";
}