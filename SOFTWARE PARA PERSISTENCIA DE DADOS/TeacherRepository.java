import com.j256.ormlite.dao.DaoManager;
import com.j256.ormlite.dao.Dao;
import java.sql.SQLException;
import com.j256.ormlite.table.TableUtils;
import java.util.List;
import java.util.ArrayList;

public class TeacherRepository
{
    private static Database database;
    private static Dao<Teacher, Integer> dao;
    private List<Teacher> loadedTeachers;
    private Teacher loadedTeacher;

    public TeacherRepository(Database database) {
        TeacherRepository.setDatabase(database);
        loadedTeachers = new ArrayList<Teacher>();
    }

    public static void setDatabase(Database database) {
        TeacherRepository.database = database;
        try {
            dao = DaoManager.createDao(database.getConnection(), Teacher.class);
            TableUtils.createTableIfNotExists(database.getConnection(), Teacher.class);
        }
        catch(SQLException e) {
            System.out.println(e);
        }
    }

    public Teacher create(Teacher teacher) {
        int nrows = 0;
        try {
            nrows = dao.create(teacher);
            if ( nrows == 0 )
                throw new SQLException("Error: object not saved");
            this.loadedTeacher = teacher;
            loadedTeachers.add(teacher);
        } catch (SQLException e) {
            System.out.println(e);
        }
        return teacher;
    }

    public void update(Teacher teacher) {
        try {
            int nrows = dao.update(teacher);
            if ( nrows == 0 )
                throw new SQLException("Error: object not updated");
            this.loadedTeacher = teacher;
        } catch (SQLException e) {
            System.out.println(e);
        }
    }

    public void delete(Teacher teacher) {
        try {
            int nrows = dao.delete(teacher);
            if ( nrows == 0 )
                throw new SQLException("Error: object not deleted");
            this.loadedTeachers.remove(teacher);
        } catch (SQLException e) {
            System.out.println(e);
        }
    }

    public Teacher loadFromId(int id) {
        try {
            this.loadedTeacher = dao.queryForId(id);
            if (this.loadedTeacher != null)
                this.loadedTeachers.add(this.loadedTeacher);
        } catch (SQLException e) {
            System.out.println(e);
        }
        return this.loadedTeacher;
    }

    public List<Teacher> loadAll() {
        try {
            this.loadedTeachers =  dao.queryForAll();
            if (this.loadedTeachers.size() != 0)
                this.loadedTeacher = this.loadedTeachers.get(0);
        } catch (SQLException e) {
            System.out.println(e);
        }
        return this.loadedTeachers;
    }

}
