autocmd Filetype c setlocal tabstop=2 softtabstop=2 shiftwidth=2 noexpandtab
augroup project
  autocmd!
  autocmd BufRead,BufNewFile *.h,*.c set filetype=c
augroup END