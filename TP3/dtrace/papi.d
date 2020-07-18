/*cpc:::PAPI_tot_ins-user-5000*/
/*cpc:::PAPI_tot_cyc-user-5000*/
/*cpc:::PAPI_l1_dcm-all-5000*/
/*cpc:::PAPI_l2_dcm-all-5000*/

/*pid$target:histogram_equalization:_Z8parallelihiPh:entry*/
/*pid$target:histogram_equalization:_Z10sequentialhiPh:entry*/


pid$target:histogram_equalization:sequential:entry
{
    equalizing = 1;
}

pid$target:histogram_equalization:sequential:return
{
    equalizing = 0;
}

pid$target:histogram_equalization:parallel:entry
{
    equalizing = 1;
}

pid$target:histogram_equalization:parallel:return
{
    equalizing = 0;
}

cpc:::PAPI_l2_dcm-all-5000

/pid == $target && equalizing/
{
    @[ufunc(arg1)] = count();
    @total = count();
}