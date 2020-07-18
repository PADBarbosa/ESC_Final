#pragma D option quiet

histogram_equalization$target:::equalization-start
{
        self->start = timestamp;
}

histogram_equalization$target:::computing-done
{
        self->computing_done = timestamp;
}

histogram_equalization$target:::normalizing-done
{
        self->normalizing_done = timestamp;
}

histogram_equalization$target:::mapping-done
{
        self->mapping_done = timestamp;
        computing_time = self->computing_done - self->start;
        normalizing_time = self->normalizing_done - self->computing_done;
        mapping_time = self->mapping_done - self->normalizing_done;
        total_time = self->mapping_done - self->start;
        printf("Computing: %dns (%d%%)\nNormalizing: %dns (%d%%)\nMapping: %dns (%d%%)\nTotal: %dns\n",
         computing_time, (100*computing_time + total_time/2)/total_time,
         normalizing_time, (100*normalizing_time + total_time/2)/total_time,
         mapping_time, (100*mapping_time + total_time/2)/total_time,
         total_time);
}