all: clean
	cp -R ../../../Source/* ./
	find ./ -type f | grep .cc | xargs  rm 
	find ./ -type f | grep .cpp | xargs  rm 
	rm -rf ./Model/*/TFLite

clean:
	rm -rf Common/
	rm -rf Model/
	rm -rf Module/
	rm -rf Tracking/
