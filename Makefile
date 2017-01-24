all:
	cd _dataServer; make
	cd _remoteClient; make

clean:
	cd _dataServer; make clean
	cd _remoteClient; make clean

