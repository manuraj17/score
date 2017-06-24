# Score

To compile:
```bash 
g++ score.cpp -lcurl -std=c++11 -o score
```

Usage: 
```bash
  ./a.out <username>
```

Example: 
```
./score manuraj17 
```

### Dependencies 

 1. json.h - https://github.com/nlohmann/json
 2. curl.h - `sudo apt install libcurl4-openssl-dev`
