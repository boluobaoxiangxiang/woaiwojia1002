modeldir   = @prefix@/lib/cabocha/model
model_DATA = @CABOCHA_MODEL_LIST@ @CABOCHA_TXTMODEL_LIST@
EXTRA_DIST = @CABOCHA_TXTMODEL_LIST@
CLEANFILES = *.model *.sjis-model
SUFFIXES   = .model .gz

%.model:
	$(YAMCHA_LIBEXEC_DIR)/mkmodel $*.txtmodel.gz $*.model

%.sjis-model:
	zcat $*.txtmodel.gz | nkf -s > $*-sjis.txtmodel
	$(YAMCHA_LIBEXEC_DIR)/mkmodel $*-sjis.txtmodel $*.sjis-model
	rm -f $*-sjis.txtmodel
