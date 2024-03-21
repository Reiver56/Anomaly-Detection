# Lista dei programmi
PROGRAMS = Database-set Test-Generator Get-Average Get-Covariance Get-Detection

.PHONY: all $(PROGRAMS) clean run kill

all:
	for dir in $(PROGRAMS); do \
		$(MAKE) -C $$dir; \
	done

$(PROGRAMS):
	$(MAKE) -C $@

clean:
	for dir in $(PROGRAMS); do \
		$(MAKE) -C $$dir clean; \
	done

server:
	./start_server.sh
run:
	./run_services.sh
stop:
	./stop_services.sh