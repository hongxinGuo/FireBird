# FireBird

### 当前状态
发行Version0.30版本。

Version0.30：修改各Exchange的数据结构。添加simdjson支持。使用vcpkg管理第三方库（concurrentqueue、concurrencpp、openssl、zlib、nlohmann-json、ms-gsl).

Version0.29:新增一独立程序WatchDog，用于监测主程序FireBird是否在执行。如果主程序意外中止的话，自动重新启动。

Version0.28:固定了数据表系统结构，不再因为添加功能而导致表结构发生变化。添加接收腾讯日线数据的功能。

Version 0.27:增加自动判断是否有权读取数据的功能，自动添加不允许读取数据的交易所名称。

### 欢迎使用FireBird
FireBird从网络股票行情服务器中提取实时行情和历史数据，将处理后的信息存储于数据库中。

#### 功能
1.分别从新浪、腾讯和网易实时行情服务器中提取实时交易数据，保证读取到所有的Level 1数据。

2.从Finnhub.io、Tiingo.com中收取美国股票市场的各种数据。

3.分析提取的实时数据其具体交易情况。

4.从网易历史数据服务器和腾讯历史数据服务器中提取日线历史数据。

#### 使用方法

###### 系统设置
参看\Docs目录中的安装手册。

### 致谢
1.GoogleTest(https://github.com/google/googletest)。

2.OpenSSL(https://www.openssl.org/)。

3.ixWebSocket(https://github.com/machinezone/IXWebSocket)。

4.readerwriterqueue(https://github.com/cameron314/readerwriterqueue).

5.Nlohmann json(https://github.com/nlohmann/json)。

6.Google benchmark(https://github.com/google/benchmark)。

7.simdjson(https://github.com/simdjson/simdjson)。

8.zlib(https://github.com/madler/zlib)。

9.concurrentqueue(https://github.com/cameron314/concurrentqueue).

10.concurrencpp(https://github.com/David-Haim/concurrencpp)




