#!/usr/bin/ruby
eval 'm=Hash.new(p=0);'+ARGF.read.gsub(/./,
     '>' => 'p+=1;',
     '<' => 'p-=1;',
     '+' => 'm[p]+=1;',
     '-' => 'm[p]-=1;',
     '.' => 'putc m[p] if m[p]!=0;',
     ',' => 'm[p]=STDIN.getbyte if !STDIN.eof;',
     '[' => 'loop do;',
     ']' => 'break if ((m[p]&=255)==0);end;')
