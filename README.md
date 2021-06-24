## Folder Structure
### 1. Source : IOS c++ source
 - Common
    - Debug : 디버깅 내용을 로그에 출력하도록 만든 함수 모음.
    - File : file 입출력.
    - Image : 이미지 resize, crop, 이미지 저장. opencv 모듈을 제거 하기 위해 이미지 클래스 생성함.
    - Parameter : 모델을 사용하는데 필요한 설정값 - 모델 입력 크기, 춣력 크기, threshold 
    - PostProcessing : non maximum suppression 구현 -> 지금은 모델 안에 들어 가 있어서 필요 없음.
    - Time : 시간 특정 함수. chrono 구현됨.
 - Model :모델 inference
    - Common :  공통 모듈
        - Impl : 실제 구현부 (bridge 패턴을 위함.)
        - Info : 모델 정보 저장
        - Result : inference 결과 정보 저장 (vector 형태 리스트)
        - TFLite : 텐서플로우 구현부
    - Classification
        - Impl
        - Info
        - Result
        - TFLite
    - Detection
        - Impl
        - Info
        - Result
        - TFLite
 - Module
    - DetectionToClassification
        - classification모델과 detection 모델을 이용하여 구현.
        - 원본 이미지에서 detecting된 영역을 잘라 내어 fine grained classification 에 넣음.
        - Result :  결과 리스트
 - Tracking : 사각 이미지 영역을 tracking 하는 모듈
    - CMT
    - GoogleObjectTracker

### 2. Build
 - Android
    - ContinuumAIAndroidUnityBuild : android aar project
        - Example의 tfilte , customview, env 폴더에서 tflite, env 를 공통영역인 ContinuumAIAndroidAAR 로 이동.
        - Result 삭제 : 각각 내부에 element 형식의 클래시 정의함.
        - IOS 와 별개로 구현 코드가 내부에 존재함.
    - 각각이 모듈을 확인할 수 있도록 만든 sample app.
        - image_classification
        - object_detection
            - 파일 변경
                - Classifier.java -> Detector.java
                - TFLiteObjectDetectionAPIModel.java ->DetectorFloatMobileNet.java
                - Tracking 관련 코드는 포함하지 않음
            - Gpu 가속 시 충분히 빠르다고 판단함.
        - pose_estimation
            - reference 의 PoseEstimationForMobileInJava를 변경하여 작성함.
            - PoseEstimationFloatInception
                - edvardHua 의 모델이 동작하는 클래스
            - PoseEstimationFloatMobileNet
                - 컨티넘 custom 모델이 동작하는 클래스
            - 입출력 shape이 다르기 때문에 수정함.
            - 관절 순서가 다르기 때문에 관절 순서를 수정함.
            - 화면과 맞추기 위한 getRatio() 함수 추가.

각각의 프로젝트 setting.gradle 에 추가
```python
include ':app', ':continuumaiandroidaar'
project(':continuumaiandroidaar').projectDir = new File('../ContinuumAIAndroidAAR/app')
```

 - IOS
    - DetectToClassify : detection 과 classification 관련 모듈이 빌드되는 xcode projects
    - Frameworks : opencv framework
    - TF_Library : tensorflow-lite cpu library file (tensorflow 1.12 version)
    - ToDo : IOS gpu 업데이트

## IOS

### 1. How to build
 - git clone Tensorflow repository
 - checkout branch r1.13 
 - cd tensorflow/tensorflow/lite/tools/make/ 
 - download_dependencies.sh 
 - build_ios_universal_lib.sh
 - Only arm64-v8a

### 2. framework project setting
 - Config.xconfig : modified your tensorflow repository path.
 - add framework : Accelerate.framework
 - Header Search Paths
    - tensorflow/tensorflow/lite/tools/make/downloads/
    - tensorflow/tensorflow/lite/tools/make/downloads/flatbuffers/include
 - Library Search Paths : ../TF_library/
 - Framework Search Paths : ../Frameworks/iOS
 - Other Linker Flags : -force_load ../TF_library/libtensorflow-arm64.a

### 3. reference
 - https://www.tensorflow.org/lite/guide/build_ios
 - https://github.com/tensorflow/tensorflow/tree/master/tensorflow/lite
 - https://github.com/tensorflow/tensorflow/tree/master/tensorflow/lite/examples/ios
 - https://github.com/gnebehay/CppMT
    - tracking 관련 코드는 업데이트 되지 않은 상태


## Android

### 1. How to build
 - bazel build aar file or download aar file (2. reference)
 - open andriod projects
 - gradle 에서 tensorflow-lite aar 파일 import
 - build
 - 결과 aar 파일 안에  unity-class.jar 삭제

### 2. Class
```shell
├── BoundingBox.java
├── CADetectToClassifyResult.java
├── ClassificationResult.java
├── DetectionResult.java
├── env
│   ├── BorderedText.java
│   ├── ImageUtils.java
│   ├── Logger.java
│   └── Size.java
└── tflite
    ├── Classifier.java
    ├── ClassifierFloatMobileNet.java
    ├── ClassifierQuantizedMobileNet.java
    ├── Detector.java
    ├── DetectorFloatMobileNet.java
    ├── DetectorQuantizedMobileNet.java
    ├── PoseEstimation.java
    ├── PoseEstimationFloatInception.java
    └── PoseEstimationFloatMobileNet.java
```
 1. env : 공통 모듈
 2. tflite 
    - image_classification
        - Classifier.java
            - recognizeImage() : inference 하는 함수    
            - Recognition : 결과 저장 inner class 위치 정보에 대한 location 변수는 있지만 사용하지 않음.
        - ClassifierFloatMobileNet.java
            - Classifier 상속
            -  gpu 를 위한 quantization 되지 않은 모델
        - ClassifierQuantizedMobileNet.java
            - Classifier 상속
            - cpu 를 위한 quantization이 된 모델
    - object_detection
        - Detector.java
            - Recognition : 결과 저장 inner class
            - recognizeImage() : inference 하는 함수
        - DetectorFloatMobileNet.java
            - Detector 상속
            - gpu 를 위한 quantization 되지 않은 모델 
        - DetectorQuantizedMobileNet.java
            - Detector 상속
            - cpu 를 위한 quantization이 된 모델
    - pose_estimation
        - PoseEstimation.java
            - classifyFrame)() : inference 하는 함수
            - Recognition : 결과 저장 inner class
        - PoseEstimationFloatInception.java
            - PoseEstimation 상속
            - edvardHua 저장소의 모델을 위함.
        - PoseEstimationFloatMobileNet.java
            - PoseEstimation 상속
            - gpu 를 위한 quantization이 된 모델, 컨티넘 custom 모델을 위함.
    - 공통함수 
        - loadLabelList()
            - 라벨 파일 로드함.
            - detection classification 모두 한 파일에 라벨이 개행 문자마다 구분되는 구조임.
            - asset에서 추가함. -> asset이 아닌 다른 모델 경로에서도 추가 가능함.
        - loadModelFile()
            - 모델 파일 로드함.
            - asset에서 추가함. -> asset이 아닌 다른 모델 경로에서도 추가 가능함.
                - Interpreter 클래스의 생성자가 File 타입을 지원함.
        - convertBitmapToByteBuffer()
            - Bitmap을 ByteBuffer로 변환하는 함수.
            - 모델 입력에 따라 addPixelValue() 함수 구현이 달라짐.

### 3. reference
 - https://github.com/tensorflow/tensorflow/tree/master/tensorflow/lite/examples/android
 - https://github.com/tensorflow/tensorflow/tree/master/tensorflow/lite
 - https://github.com/tensorflow/tensorflow/tree/master/tensorflow/lite/java
 - https://mvnrepository.com/artifact/org.tensorflow/tensorflow-lite?repo=springio-libs-release
 - https://docs.unity3d.com/kr/current/Manual/PluginsForAndroid.html
 - https://github.com/tensorflow/examples
 - https://github.com/edvardHua/PoseEstimationForMobile
 - https://github.com/sauvignon1989/PoseEstimationForMobileInJava
