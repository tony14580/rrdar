/*
 * time.cpp
 *
 *  Created on: Apr 26, 2021
 *      Author: jelavice
 */
#include "icp_localization_ros2/common/time.hpp"
#include <rcl/time.h>
#include <time.h>
#include <chrono>
#include <cerrno>
#include <cstring>
#include <string>

namespace icp_loco{
double readable(const Time &t){
  return toUniversal(t - Duration(637060590580261258)) / 10000.0;

}



Duration fromSeconds(const double seconds) {
  return std::chrono::duration_cast<Duration>(
      std::chrono::duration<double>(seconds));
}

double toSeconds(const Duration duration) {
  return std::chrono::duration_cast<std::chrono::duration<double>>(duration)
      .count();
}

double toSeconds(const std::chrono::steady_clock::duration duration) {
  return std::chrono::duration_cast<std::chrono::duration<double>>(duration)
      .count();
}

Time fromUniversal(const int64 ticks) { return Time(Duration(ticks)); }

int64 toUniversal(const Time time) { return time.time_since_epoch().count(); }

std::ostream& operator<<(std::ostream& os, const Time time) {
  os << std::to_string(toUniversal(time));
  return os;
}

Duration fromMilliseconds(const int64 milliseconds) {
  return std::chrono::duration_cast<Duration>(
      std::chrono::milliseconds(milliseconds));
}

std::string toString(const Time &time){
  return std::to_string(toUniversal(time));
}

rclcpp::Time toRos(Time time) {
  int64_t uts_timestamp = toUniversal(time);
  int64_t ns_since_unix_epoch =
      (uts_timestamp -
       kUtsEpochOffsetFromUnixEpochInSeconds *
           10000000ll) *
      100ll;
  // std::cout<< ns_since_unix_epoch<<"\n";
  ::rclcpp::Time ros_time(ns_since_unix_epoch); 
  return ros_time;
}

Time fromRos(const ::rclcpp::Time& time) {
  // The epoch of the ICU Universal Time Scale is "0001-01-01 00:00:00.0 +0000",
  // exactly 719162 days before the Unix epoch.
  return fromUniversal(
      (
       kUtsEpochOffsetFromUnixEpochInSeconds) *
          10000000ll +
      (time.nanoseconds() + 50) / 100);  // + 50 to get the rounding correct.
}


} // namespace icp_loco
