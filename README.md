# FS16-to-ASM-Compiler
Compiler for FS16 to ASM

# Language
```
<program>  ->     <vars> <block>
<block>       ->      Begin <vars> <stats> End
<vars>          ->      empty | Var Identifier <mvars> 
<mvars>     ->     empty | : : Identifier <mvars>
<expr>        ->      <M> + <expr> | <M>
<M>              ->     <T> - <M> | <T>
<T>              ->      <F> * <T> | <F> / <T> | <F>
<F>              ->      - <F> | <R>
<R>              ->      [ <expr> ] | Identifier | Number   
<stats>         ->      <stat>  <mStat>
<mStat>       ->      empty | <stat>  <mStat>
<stat>           ->      <in> | <out> | <block> | <if> | <loop> | <assign>
<in>              ->      Scan : Identifier .
<out>            ->      Print [ <expr>  ] .
<if>               ->      [ <expr> <RO> <expr> ]  Iff <block>             
<loop>          ->      Loop [ <expr> <RO> <expr> ] <block>
<assign>       ->      Identifier == <expr> .
<RO>            ->      >=> | <=< | = |  > | <  |  =!=   
```
# Details
Scanner is implemented using FSA table. Parser builds a top down tree which is then recursively traversed by semantic.cpp and generates scope
and code generation.

# Invocation
comp [file]

# Errors
Errors will be caught when there is an invalid character, a syntax error or a scoping error.
