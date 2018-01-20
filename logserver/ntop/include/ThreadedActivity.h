/*
 *
 * (C) 2013-17 - ntop.org
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#ifndef _THREADED_ACTIVITY_H_
#define _THREADED_ACTIVITY_H_

#include "ntop_includes.h"
//线程，runScript脚本运行
class ThreadedActivity {
 private:
  pthread_t pthreadLoop;
  char path[MAX_PATH];
  NetworkInterface *iface;
  u_int32_t periodicity;
  bool align_to_localtime;

//  static void runScript(char *path, u_int32_t when);
//  static u_int32_t roundTime(u_int32_t now, u_int32_t rounder, int32_t offset_from_utc);

//  void periodicActivityBody();//周期
//  void aperiodicActivityBody();//非定期
//  void uSecDiffPeriodicActivityBody();

 public:
  ThreadedActivity(const char* _path, NetworkInterface *_iface = NULL,
		   u_int32_t _periodicity_seconds = 0, bool _align_to_localtime = false);
  ~ThreadedActivity();
//  void activityBody();

//  void run();
};

#endif /* _THREADED_ACTIVITY_H_ */
