all: clean
	cp -R ../../../../Source/* ./
	find ./ -type f | grep .cc | xargs  rm 
	find ./ -type f | grep .cpp | xargs  rm 
	rm -rf ./Model/Detection
	rm -rf ./Model/PoseEstimation
	rm -rf ./Module/
	rm -rf ./Tracking/
	rm -rf ./Model/*/TFLite

clean:
	rm -rf Common/
	rm -rf Model/
