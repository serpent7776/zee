#Rakefile for zee v0.41
import 'Rakefile.inc';

CPP_FLAGS=['-std=gnu99 -Os'];
OPTS={
  'MKBUILDDIRS'=>true,	# whether to create bin and obj directories
  'PERL'=>false,	# whether to enable perl usage
}

Tasks={
  'zee'=>{
    :type=>:exec,
    :sources=>%w[main.c zee.c],
    :link_flags=>[],
  }
}
SourceFlags={
}
Install=[
]
