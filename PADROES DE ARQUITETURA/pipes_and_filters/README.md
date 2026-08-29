# Exercício sobre Pipes and Filters

## Para compilar a pipeline inteira
```
g++ -std=c++17 -Wall -O2 -o pipeline.exe src\main.cpp src\ReadFilter.cpp src\CleanFilter.cpp src\Pipeline.cpp src\ReportFilter.cpp src\SumFilter.cpp src\TransformFilter.cpp
```

## Para executar a pipeline em um arquivo .csv
```
./pipeline <nome_do_arquivo>.csv
```

## Para compilar os testes
```
g++ -std=c++17 -Wall -O2 -o run_tests.exe tests\test_filters.cpp  src\ReadFilter.cpp src\CleanFilter.cpp src\Pipeline.cpp src\ReportFilter.cpp src\SumFilter.cpp src\TransformFilter.cpp
```

## Para executar os testes
```
./run_tests
```