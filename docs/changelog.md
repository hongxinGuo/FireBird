2023.12.09 Version0.30

修改各exchange的数据结构；添加simdjson支持。
使用vcpkg管理第三方库（concurrentqueue、concurrencpp、openssl、zlib、nlohmann-json、ms-gsl).

2023.05.23 Version0.29

1.新增一独立程序WatchDog，用于监测主程序FireBird是否在执行。如果主程序意外中止的话，自动重新启动之。

Version0.28

固定了数据表系统结构，不再因为添加功能而导致表结构发生变化。添加接收腾讯日线数据的功能。

Version 0.27

增加自动判断是否有权读取数据的功能，自动添加不允许读取数据的交易所名称。