#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

int Check_Shared_Disk_stat (int rank, int size, const char *directory)
{
	if (size > 1)
	{
		int howmany;
		char name[MPI_MAX_PROCESSOR_NAME];
		char name_master[MPI_MAX_PROCESSOR_NAME];
		int len;

		if (rank == 0)
		{
			PMPI_Get_processor_name(name, &len);
			PMPI_Bcast (name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, 0, MPI_COMM_WORLD);
		}
		else
			PMPI_Bcast (name_master, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, 0, MPI_COMM_WORLD);

		if (rank == 0)
		{
			struct stat s;
			unsigned res = 1;
			unsigned length = strlen(directory)+strlen("/shared-disk-testXXXXXX")+1;
			char *template = malloc (length*sizeof(char));
			assert(template != NULL);
			sprintf (template, "%s/shared-disk-testXXXXXX", directory);
			int ret = mkstemp (template);
			assert (ret >= 0);
			PMPI_Bcast (&length, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
			PMPI_Bcast (template, length, MPI_CHAR, 0, MPI_COMM_WORLD);
			ret = stat (template, &s);
			PMPI_Bcast (&s, sizeof(struct stat), MPI_BYTE, 0, MPI_COMM_WORLD);
			PMPI_Reduce (&res, &howmany, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
			unlink (template);
			free (template);
		}
		else
		{
			int res;
			int ret;
			char *template;
			unsigned length;
			struct stat master_s, s;
			PMPI_Bcast (&length, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
			template = malloc (length*sizeof(char));
			assert (template != NULL);
			PMPI_Bcast (template, length, MPI_CHAR, 0, MPI_COMM_WORLD);
			PMPI_Bcast (&master_s, sizeof(struct stat), MPI_BYTE, 0, MPI_COMM_WORLD);
			ret = stat (template, &s);
			res = ret == 0 &&
			  (master_s.st_uid == s.st_uid) &&
			  (master_s.st_gid == s.st_gid) &&
			  (master_s.st_ino == s.st_ino) &&
			  (master_s.st_mode == s.st_mode);
			PMPI_Reduce (&res, NULL, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
			free (template);
		}
		return howmany == size;
	}
	else
		return 1;
}

int Check_Shared_Disk_openread (int rank, int size, const char *directory)
{
	if (size > 1)
	{
		int howmany;
		char name[MPI_MAX_PROCESSOR_NAME];
		char name_master[MPI_MAX_PROCESSOR_NAME];
		int len;

		if (rank == 0)
		{
			PMPI_Get_processor_name(name, &len);
			PMPI_Bcast (name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, 0, MPI_COMM_WORLD);
		}
		else
			PMPI_Bcast (name_master, MPI_MAX_PROCESSOR_NAME, MPI_CHAR, 0, MPI_COMM_WORLD);

		if (rank == 0)
		{
			unsigned res = 1;
			unsigned length = strlen(directory)+strlen("/shared-disk-testXXXXXX")+1;
			char *template = malloc (length*sizeof(char));
			assert(template != NULL);
			sprintf (template, "%s/shared-disk-testXXXXXX", directory);
			int ret = mkstemp (template);
			assert (ret >= 0);
			write (ret, name, strlen(name)+1);
			PMPI_Bcast (&length, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
			PMPI_Bcast (template, length, MPI_CHAR, 0, MPI_COMM_WORLD);
			PMPI_Reduce (&res, &howmany, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
			unlink (template);
			free (template);
		}
		else
		{
			int fd;
			unsigned res = 0;
			char *template;
			unsigned length;
			PMPI_Bcast (&length, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
			template = malloc (length*sizeof(char));
			assert (template != NULL);
			PMPI_Bcast (template, length, MPI_CHAR, 0, MPI_COMM_WORLD);
			fd = open(template, O_RDONLY);
			if (fd >= 0)
			{
				read (fd, name, sizeof(name));
				if (strcmp (name, name_master) == 0)
					res = 1;
			}
			PMPI_Reduce (&res, NULL, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
			free (template);
		}
		return howmany == size;
	}
	else
		return 1;
}

int main (int argc, char *argv[])
{
	struct timespec tbegin, tend;
	int size, rank, res = 0;
	char *directory = (argc > 1)?argv[1]:getenv("PWD");
	MPI_Init (&argc, &argv);
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	clock_gettime (CLOCK_MONOTONIC, &tbegin);
	res = Check_Shared_Disk_stat (rank, size, directory);
	clock_gettime (CLOCK_MONOTONIC, &tend);
	if (rank == 0)
		printf ("Rank %d out of %d IsShared (%s) = %d, operation took %ld seconds\n", rank+1, size, directory, res, tend.tv_sec - tbegin.tv_sec); 
	MPI_Finalize ();
	return 0;
}
