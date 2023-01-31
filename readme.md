# FireBird

### 当前状态
发行Version0.28版本。

Version0.28:固定了数据表系统结构，不再因为添加功能而导致表结构发生变化。添加接收腾讯日线数据的功能。

Version 0.27:增加自动判断是否有权读取数据的功能，自动添加不允许读取数据的交易所名称。

### 欢迎使用FireBird
FireBird从网络股票行情服务器中提取实时行情和历史数据，将处理后的信息存储于数据库中。

#### 功能
1.分别从新浪、腾讯和网易实时行情服务器中提取实时交易数据，保证读取到所有的Level 1数据。

2.从Finnhub.io、Tiingo.com中收取美国股票市场的各种数据。

3.分析提取的实时数据其具体交易情况。

4.从网易历史数据服务器中提取日线历史数据。

#### 使用方法

###### 系统设置
参看\Docs目录中的安装手册。

### 致谢
1.GoogleTest(https://github.com/google/googletest)。

2.OpenSSL(https://www.openssl.org/)。

3.ixWebSocket(https://github.com/machinezone/IXWebSocket)。

4.Visual Leak Detector(https://github.com/KindDragon/vld/tree/v2.5.1)。

5.Nlohmann json(https://github.com/nlohmann/json)。

6.Google benchmark(https://github.com/google/benchmark)。

7.curl(https://github.com/curl/curl)。

8.zlib(https://github.com/madler/zlib)。




