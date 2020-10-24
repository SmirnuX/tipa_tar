all:
	gcc tipatar.c make_tar.c unmake_tar.c -o ttar
	
sanitized: 
	gcc tipatar.c make_tar.c unmake_tar.c -o ttar -fsanitize=address
