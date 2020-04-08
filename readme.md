# FireBird StockAnalysis

### 声明
Version 0.10.0已可用。

### 欢迎使用FireBird StockAnalysis
FireBird StockAnalysis从网络股票行情服务器中提取实时行情和历史数据，将处理后的信息存储于数据库中。

#### 功能
1.分别从新浪、腾讯和网易实时行情服务器中提取实时交易数据。每三秒轮询一次，保证读取到所有的Level 1数据。

2.分析提取的实时数据其具体交易情况。

3.从网易历史数据服务器中提取日线历史数据。

4.采用MySQL数据库存储提取和分析后的股票行情。

#### 使用方法

###### 系统设置
1.安装MySQL社区版。设置两个默认用户hxguo和Test，默认密码分别为hxguo和test。
将Database目录中的DataStructure.sql导入数据库。

2.在Windows管理工具中添加两个ODBC64位驱动：分别对应hxguo和Test。如下图所示：

![Image Text](Img/Microsoft%20ODBC.png)

![Image Text](Img/Microsoft%20ODBC%20Option1.png)

![Image Text](Img/Microsoft%20ODBC%20Option2.png)

3.如果您的系统中没有Visual Studio环境的话，需要下载VS2019再发行包(https://visualstudio.microsoft.com/zh-hans/downloads/?rr=https%3A%2F%2Fsupport.microsoft.com%2Fen-us%2Fhelp%2F2977003%2Fthe-latest-supported-visual-c-downloads )，本系统默认编译为64位系统。

4.将bin目录中的StockAnalysis.exe放入工作目录中。




