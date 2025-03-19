/*
 * Copyright 1996-2024 Cyberbotics Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <webots/motor.h>
#include <webots/position_sensor.h>
#include <webots/robot.h>

#define TIME_STEP 32

static float positions[5][3] = {{0.50, -2.2, 2.56}, {0.41, -2.2, 2.56}, {0.31, -2.2, 2.56}, {0.21, -2.2, 2.56}, {0, -1.770800, 1.57}};

enum Joints { JOINT1, JOINT2, JOINT3, JOINT4, JOINT5, JOINT6, JOINT7, FINGER };
enum Blocks { BLOCK1, BLOCK2, BLOCK3 };
enum HandCommands { OPEN_HAND, CLOSE_HAND };

static WbDeviceTag motors[9];

void hand_control(int command) {
  // based on the state, opens or closes the gripper
  wb_motor_set_position(motors[FINGER], (command == OPEN_HAND) ? 0.04 : 0.01);

  // delay for the action to take place
  wb_robot_step(TIME_STEP * 10);
}

void move_to_position(int position) {
  wb_motor_set_position(motors[JOINT2], positions[position][0]);
  wb_motor_set_position(motors[JOINT4], positions[position][1]);
  wb_motor_set_position(motors[JOINT6], positions[position][2]);
  // delay for the movement to take place
  wb_robot_step(TIME_STEP * 20);
}


int main(int argc, char **argv) {
  wb_robot_init();

  char device_name[30];
  // retrieve the motor references of all the joints
  for (int i = 0; i < 7; ++i) {
    const char *prefix = "panda_joint";
    sprintf(device_name, "%s%d", prefix, i + 1);
    motors[i] = wb_robot_get_device(device_name);
  }

  motors[7] = wb_robot_get_device("panda_finger::right");
  hand_control(OPEN_HAND);
  wb_robot_step(TIME_STEP * 50);
  move_to_position(0);
  wb_robot_step(TIME_STEP * 50);
  hand_control(CLOSE_HAND);
  wb_robot_step(TIME_STEP * 50);
  move_to_position(1);
  wb_robot_step(TIME_STEP * 50);
  move_to_position(2);
  wb_robot_step(TIME_STEP * 50);
  move_to_position(3);
  wb_robot_step(TIME_STEP * 50);
  move_to_position(0);
  wb_robot_step(TIME_STEP * 50);
  hand_control(OPEN_HAND);
  wb_robot_step(TIME_STEP * 50);
  move_to_position(4);
  wb_robot_step(TIME_STEP * 50);
  
  wb_robot_cleanup();

  return 0;
}
