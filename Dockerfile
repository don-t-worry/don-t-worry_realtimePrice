# 베이스 이미지로 Ubuntu 사용
FROM ubuntu:20.04

# 필요한 패키지 설치 (비대화형 모드)
ENV DEBIAN_FRONTEND=noninteractive

# 필요한 패키지 설치
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libssl-dev \
    libcurl4-openssl-dev \
    libyaml-cpp-dev \
    libboost-all-dev \
    && apt-get clean

# 작업 디렉토리 생성
WORKDIR /app

# CMakeLists.txt 및 소스 파일 복사
COPY . .

# CMake로 빌드
RUN rm -rf main/build && mkdir -p main/build && cd main/build && cmake .. && make

# 컨테이너 시작 시 실행할 명령어 설정
CMD ["./main/build/bin/dontworry_stockPrice"]