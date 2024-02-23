# Lista dei programmi
PROGRAMS = Database-set Test-Generator Get-Average Get-Covariance Get-Detection

.PHONY: all $(PROGRAMS) clean

all: $(PROGRAMS)

$(PROGRAMS):
	$(MAKE) -C $@

clean:
	for dir in $(PROGRAMS); do \
		$(MAKE) -C $$dir clean; \
	done

run: $(PROGRAMS)

$(PROGRAMS):
	$(MAKE) -C  $@ run
