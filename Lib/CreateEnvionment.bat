cd c:\
md StockAnalysis
cd stockAnalysis
md Test Data
cd Test Data
md Benchmark
cd Benchmark
copy %(Solution)\Lib\Test Data\Benchmark C:\StockAnalysis\Test Data\Benchmark
cd ..
cd ..
copy %(Solution)\Lib\bin_release\*.dll c:\StockAnalysis
