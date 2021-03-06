! Test interface-block
!      interface-block  is  interface-stmt
!                             [ interface-specificiation ] ...
!                           end-interface-stmt
!
!      interface-specification  is  interface-body
!                               or  procedure-stmt
!
!      interface-stmt  is  INTERFACE [ generic-spec ]
!                      or  ABSTRACT INTERFACE
!
!      end-interface-stmt  is  END INTERFACE [ generic-spec ]
!
! Not tested here: interface-body, procedure-stmt, and generic-spec.
!
! Passes gfortran test when the abstract interface section is commented out
!
! Include none of the optional parts.
interface
end interface

! Include a generic-spec in the interface-stmt and end-interface-stmt
interface foo
end interface foo

! Try an abstract interface

abstract interface
end interface

! Include an optional interface-specification
interface
   subroutine sub1()
   end subroutine sub1
end interface

! Include multiple optional interface-specification
interface
   subroutine sub2()
   end subroutine sub2
   function foo()
   end function foo
end interface

end

