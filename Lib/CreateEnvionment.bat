cd c:\
md FireBird
cd FireBird
md Test Data
cd Test Data
md Benchmark
cd Benchmark
copy %(Solution)\Lib\Test Data\Benchmark C:\FireBird\Test Data\Benchmark
cd ..
cd ..
copy %(Solution)\Lib\bin_release\*.dll c:\FireBird
