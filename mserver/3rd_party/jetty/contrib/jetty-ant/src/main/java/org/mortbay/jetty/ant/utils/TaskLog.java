// ========================================================================
// Copyright 2006-2007 Sabre Holdings.
// ------------------------------------------------------------------------
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ========================================================================

package org.mortbay.jetty.ant.utils;

import java.text.SimpleDateFormat;
import java.util.Date;

import org.apache.tools.ant.Task;

/**
 * Provides logging functionality for classes without access to the Ant project
 * variable.
 * 
 * @author Jakub Pawlowicz
 */
public class TaskLog
{

    private static Task task;

    private static SimpleDateFormat format = new SimpleDateFormat(
            "yyyy-MM-dd HH:mm:ss.SSS");

    public static void setTask(Task task)
    {
        TaskLog.task = task;
    }

    public static void log(String message)
    {
        task.log(message);
    }

    public static void logWithTimestamp(String message)
    {
        task.log(format.format(new Date()) + ": " + message);
    }
}
