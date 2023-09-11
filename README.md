# RIP
# 환경 설정
## 1. Anaconda Prompt 실행
## 2. 가상 환경 만들기
- 가상환경 만들기
```
conda create -n [환경 이름] python=3.8
```
- 가상환경 들어가기
```
conda activate [환경 이름]
```
- 가상환경 나오기
```
conda deactivate 
```
## 3. 모듈 설치
### 가상환경에 설치하는 것을 권장합니다
- conda
```
conda install numpy pyserial opencv
conda install jupyter notebook
```
- pip
```
pip install torch==1.13.1 torchvision==0.14.1 torchaudio==0.13.1
pip install ultralytics
```
## 4. Jupyter notebook 실행
```
jupyter notebook
```
## 5. 파일 저장해놓은 경로에서 코드 실행해보기
