SUBDIRS = prototype lkm server

subdirs:
	for dir in $(SUBDIRS); do \
		cd $$dir;	\
		$(MAKE); \
		cd - ;\
	done

clean:
	for dir in $(SUBDIRS); do \
		cd $$dir;	\
		$(MAKE) clean; \
		cd - ;\
	done
