#include <unistd.h>
#include "serialPort/SerialPort.h"
#include "unitreeMotor/unitreeMotor.h"
#include <algorithm>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <yaml-cpp/yaml.h> 
#include <fstream> // 用于 std::ofstream

#define N 56.52f    //一圈最大值
#define SERVER_IP "127.0.0.1" // 本地服务器 IP
#define PORT 8080   //端口


int main() {
  YAML::Node config = YAML::LoadFile("../yaml/A1.yaml");
  SerialPort  serial("/dev/ttyUSB0");
  MotorCmd    cmd;
  MotorData   data;
  int client_fd;
  struct sockaddr_in server_addr;

  // 创建套接字
  if ((client_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Socket creation failed");
    return -1;
  }
  // 设置服务器地址
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
    perror("Invalid address or address not supported");
    close(client_fd);
    return -1;
  }

  //导入yaml文件参数
  float kp = config["motor"]["kp"].as<float>();
  float kd = config["motor"]["kp"].as<float>();
  float pose = config["motor"]["pose"].as<float>();
  while(true) 
  {
    YAML::Node config = YAML::LoadFile("../yaml/A1.yaml");//重新读取yaml文件

    //A1电机命令配置
    cmd.motorType = MotorType::A1;
    data.motorType = MotorType::A1;
    cmd.mode  = queryMotorMode(MotorType::A1,MotorMode::FOC);
    cmd.id    = 0;
    cmd.kp    = kp;
    cmd.kd    = kd;
    cmd.q     = pose;
    cmd.dq    = 0;
    cmd.tau   = 0.0;
    cmd.q = std::clamp(cmd.q, -N, N);
    //发送a1电机命令
    serial.sendRecv(&cmd,&data);
    //判断是否需要更新参数      send参数为01的时候进入判断并将send参数修改为0
    if (config["motor"]["send"].as<int>())
    {
      config["motor"]["send"] = 0;
      std::ofstream fout("../yaml/A1.yaml");
      fout << config;
      fout.close();
      kp=config["motor"]["kp"].as<float>();
      kd = config["motor"]["kd"].as<float>();
      pose=pose+config["motor"]["pose"].as<float>();;
    }
    
    std::cout <<  std::endl;
    std::cout <<  "motor.q: "    << data.q    <<  std::endl;
    std::cout <<  "motor.kp: "   << cmd.kp   <<  std::endl;
    std::cout <<  "motor.kd: "      << cmd.kd      <<  std::endl;
    // std::cout <<  "motor.merror: " << data.merror <<  std::endl;
    std::cout <<  std::endl;
    //UDP发送
    std::string str = std::to_string(data.q)+","+std::to_string(cmd.q)+"\n";
    const char *message = str.c_str();
    sendto(client_fd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    usleep(200);
  }
  close(client_fd);
  return 0;
}