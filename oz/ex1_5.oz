declare Test Test2 in
proc {Test X}
   case X
   of a|Z then {Browse 'case'(1)}
   [] f(a) then {Browse 'case'(2)}
   [] Y|Z andthen Y==Z then {Browse 'case'(3)}
   [] Y|Z then {Browse 'case'(4)}
   [] f(Y) then {Browse 'case'(5)}
   else {Browse 'case'(6)} end
end
