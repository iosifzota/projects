declare NewQueue Insert Delete IsEmpty
fun {NewQueue} X in q(0 X X) end
fun {Insert Q X}
   case Q of q(N S E) then E1 in
      E = X|E1
      q(N+1 S E1)
   end
end
fun {Delete Q X}
   case Q of q(N S E) then S1 in
      S = X|S1
      q(N-1 S1 E)
   end
end
fun {IsEmpty Q}
   case Q of q(N S E) then N==0 end
end

declare Q1 Q2 Q3 Q4 Q5 in
Q1 = {NewQueue}
Q2 = {Insert Q1 jam}
Q3 = {Insert Q2 egg}
local X in Q4={Delete Q3 X} {Browse X} end
local X in Q5={Delete Q4 X} {Browse X} end
   

