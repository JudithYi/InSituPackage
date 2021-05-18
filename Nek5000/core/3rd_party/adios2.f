c-------------------------------------------
c       Initialization 
c-------------------------------------------
      subroutine adios2_init()
      implicit none
      include 'SIZE'
      include 'TOTAL'
      integer dim
      dim = 2
      if(if3d) dim = 3
      call adios2_setup(dim)
      end
c-------------------------------------------
c       Write 
c-------------------------------------------
      subroutine adios2_write()
      implicit none
      include 'SIZE'
      include 'TOTAL'
      integer nvals
      nvals = lx1 * ly1 * ly1 * lelt
      if (if3d) then
            call adios2_update(xm1, ym1, zm1, pr, vx, vy, vz, t, nvals)
      else
            call adios2_update(xm1, ym1, pr, vx, vy, t, nvals)
      end if
      end
c-------------------------------------------
c       Finalization 
c-------------------------------------------
      subroutine adios2_end()
      call adios2_finalize()
      end