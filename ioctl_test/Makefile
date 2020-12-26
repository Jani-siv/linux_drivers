all: ioctl_test

app : ioctl_test.c
	$(CC) -o $@ $^
clean :
	rm ioctl_test
deploy :
	scp $^ pi@10.0.0.10:
		
