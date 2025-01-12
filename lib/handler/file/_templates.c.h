/*
 * Copyright (c) 2014 DeNA Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * lib/file/templates.c.h is automatically generated from lib/file/_templates.h
 * with command:
 *   picotemplate.pl --conf=misc/picotemplate-conf.pl lib/file/_templates.c.h
 */

static int cmpstrptr(const void *_x, const void *_y)
{
    const char *x = *(const char **)_x;
    const char *y = *(const char **)_y;
    return strcmp(x, y);
}

static h2o_buffer_t *build_dir_listing_html(h2o_mem_pool_t *pool, h2o_iovec_t path_normalized, DIR* dp)
{
    H2O_VECTOR(char *) files = {};

    { /* build list of files */
        struct dirent dent, *dentp;
        int ret;
        while ((ret = readdir_r(dp, &dent, &dentp)) == 0 && dentp != NULL) {
            if (strcmp(dent.d_name, ".") == 0 || strcmp(dent.d_name, "..") == 0)
                continue;
            h2o_vector_reserve(NULL, (void *)&files, sizeof(files.entries[0]), files.size + 1);
            files.entries[files.size++] = h2o_strdup(NULL, dent.d_name, SIZE_MAX).base;
        }
        qsort(files.entries, files.size, sizeof(files.entries[0]), cmpstrptr);
    }

    h2o_buffer_t *_;
    h2o_iovec_t path_normalized_escaped = h2o_htmlescape(pool, path_normalized.base, path_normalized.len);

    h2o_buffer_init(&_, &h2o_socket_buffer_prototype);

?<!DOCTYPE html>
?<TITLE>Index of <?= path_normalized_escaped ?></TITLE>
?<H2>Index of <?= path_normalized_escaped ?></H2>
?<UL>
?<LI><A HREF="..">Parent Directory</A>

    size_t i;
    for (i = 0; i != files.size; ++i) {
        h2o_iovec_t fn_escaped = h2o_htmlescape(pool, files.entries[i], strlen(files.entries[i]));
?<LI><A HREF="<?= fn_escaped ?>"><?= fn_escaped ?></A>
        free(files.entries[i]);
    }

    free(files.entries);
    return _;
}
