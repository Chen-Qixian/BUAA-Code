## 基于拓扑映射的并行应用通信性能优化工具

> Topology mapping based parallel application Communication Optimization System

### 👨 作者

北京航空航天大学

计算机学院

陈麒先

### 🖍️ 课题来源

北京航空航天大学 本科毕业设计

### 🕙 研究时间

2019.12.19 ~ 2020.6.4

### 📒 源代码目录

- 0-Algorithm ：拓扑映射算法及接口
  - APHiD ：APHiD算法源代码
  - LPMS ：LPMS算法源代码
  - MPIPP ：MPIPP算法源代码（开源代码）
  - QTMS ：QTMS算法源代码
  - TopoMapping ：TopoMapping算法源代码
  - TreeMatch ：TreeMatch算法源代码（开源代码）
  - wasms ：算法模型接口 包括.wasm和.js模块 可以被服务端调用
- 1-Client ：可视化工具客户端/前端
  - README ：客户端使用说明
  - TCOS ：可视化工具客户端/前端源代码 主要技术栈Vue
- 2-Server ：可视化工具服务端
  - README ：服务端使用说明
  - server ：可视化工具服务端源代码 主要技术栈node + express
- 3-TopoGet ：拓扑构建工具
  - mpiP ：修改后的通信拓扑构建工具
  - mpiP_origin ：通信捕捉工具mpiP（开源代码）
  - pingpong-test ：网络性能测量工具
  - result ：拓扑构建结果
    - netgraph ：网络拓扑构建结果
    - taskgraph：通信拓扑构建结果
- 4-ExpShell ：实验脚本（实验室代码）
  - *.sh ：shell脚本用于在天河-2执行并行应用程序开展实验
  - *.result ：适用于天河-2的格式化节点分配文件
- 5-TestCase ：测试用例（由实验室提供的早期测试用例）
- 6-ExpResult ：实验结果与可视化代码
  - *.js ：echarts可视化代码
  - result.excel ：原始实验数据记录表
- 7-Evaluation ：算法评价

***注：来自互联网或实验室与本课题强相关性代码均已标注**



### ⚠️ 可视化工具使用说明

> 可视化工具分为客户端与服务端，需要分别启动。

- 可视化工具客户端启动

```bash
# 进入客户端工程项目文件路径
cd 1-Client/TCOS

# 安装全部项目依赖
npm install

# 在localhost:8080启动客户端
npm run start
or
npm run dev

# 如果需要上线发布稳定版本，对项目进行打包构建
npm run build
```

**⚠️注意：运行项目依赖node与npm环境，以上代码均在node 12.9.1 和 npm6.10.2 环境下调测通过。**

- 可视化工具服务端启动

```bash
# 进入服务端工程项目文件路径
cd 2-Server/server

# 安装全部项目依赖
npm install

# 启动服务端
npm run start
or
npm run dev

# 服务端可以部署在服务器上
```

**⚠️注意：必须启动服务端后，登陆可视化工具网址可以正常使用其功能。**



### 🐞 Bug报告与反馈

> 如果您在使用本系统时有任何意见或建议，欢迎与项目作者 陈麒先 取得联系

**📮邮箱：chenqixian@buaa.edu.cn**

**☎️电话：010-82317814**

**🏠地址：北京市海淀区学院路37号**