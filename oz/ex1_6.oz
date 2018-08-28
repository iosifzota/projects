declare Test X Y in
proc {Test X}
   case X of f(a Y c) then {Browse Y}
   else {Browse 'case'(2)} end
end

{Test f(X Y d)}


declare Max3 Max5
proc {SpecialMax Value ?SMax}
   fun {SMax X}
      if X>Value then X else Value end
   end
end

{SpecialMax 3 Max3}
{SpecialMax 5 Max5}

{Browse [{Max3 4} {Max5 4}]}
