
TARGETS= post-commit--000 post-incoming--000

default: triggerd $(TARGETS)

# Will the following work if $TARGETS are checked in?
$(TARGETS): triggert
	rm -f *--[0-9][0-9][0-9]?*
	rm -f $@
	sed -e 's/@BN@/triggerd/' < triggert > $@ && chmod +x $@

#triggerd: trigger.cfg
