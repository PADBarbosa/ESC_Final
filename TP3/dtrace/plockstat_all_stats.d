/*pid$target:histogram_equalization:_Z8parallelihiPh:entry*/

pid$target:histogram_equalization:parallel:entry
{
    equalizing = 1;
}

pid$target:histogram_equalization:parallel:return
{
    equalizing = 0;
}



plockstat$target:::mutex-spin
/pid == $target && equalizing/
{
    @occurrences["spin"] = count();
    self->spinStart = timestamp;
}


plockstat$target:::mutex-acquire
/pid == $target && self->spinStart && equalizing/
{
    @occurrences["spin->acquire"] = count();
    @average["average time(ns): spin->acquire"] = avg(timestamp - self->spinStart);
    @hist["time(ns): spin->acquire"] = quantize(timestamp - self->spinStart);
    self->spinStart = 0;
}

plockstat$target:::mutex-block
/pid == $target && self->spinStart && equalizing/
{
    self->blockStart = timestamp;
    @occurrences["spin->block"] = count();
    @average["average time(ns): spin->block"] = avg(timestamp - self->spinStart);
    @hist["time(ns): spin->block"] = quantize(timestamp - self->spinStart);
    self->spinStart = 0;
}

plockstat$target:::mutex-acquire
/pid == $target && self->blockStart && equalizing/
{
    @occurrences["block->acquire"] = count();
    @average["average time(ns): block->acquire"] = avg(timestamp - self->blockStart);
    @hist["time(ns): block->acquire"] = quantize(timestamp - self->blockStart);
    self->blockStart = 0;
}

plockstat$target:::mutex-acquire
/pid == $target && self->blockStart == 0 && self->spinStart == 0 && equalizing/
{
    @occurrences["immediate acquire"] = count();
}
