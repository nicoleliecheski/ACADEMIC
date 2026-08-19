import java.util.Date;
import java.text.SimpleDateFormat;
import com.j256.ormlite.table.DatabaseTable;
import com.j256.ormlite.field.DatabaseField;
import com.j256.ormlite.field.DataType;

@DatabaseTable(tableName = "teacher")
public class Teacher
{
    @DatabaseField(generatedId = true)
    private int id;

    @DatabaseField
    private String fullName;

    @DatabaseField
    public int employeeId;

    @DatabaseField(dataType=DataType.DATE)
    public Date hireDate;

    @DatabaseField(dataType=DataType.DOUBLE)
    public double salary;

    public String printHireDate() {
        SimpleDateFormat dateFor = new SimpleDateFormat("dd/MM/yyyy");
        return dateFor.format(hireDate);
    }

    public int getId(){
        return this.id;
    }

    public void setId(int id){
        this.id = id;
    }

    public String getFullName(){
        return this.fullName;
    }

    public void setFullName(String fullName){
        this.fullName = fullName;
    }

    public int getEmployeeId(){
        return this.employeeId;
    }//end method getEmployeeId

    /**SET Method Propertie employeeId*/
    public void setEmployeeId(int employeeId){
        this.employeeId = employeeId;
    }//end method setEmployeeId

    /**GET Method Propertie hireDate*/
    public Date getHireDate(){
        return this.hireDate;
    }//end method getHireDate

    /**SET Method Propertie hireDate*/
    public void setHireDate(Date hireDate){
        this.hireDate = hireDate;
    }//end method setHireDate

    /**GET Method Propertie salary*/
    public double getSalary(){
        return this.salary;
    }//end method getSalary

    /**SET Method Propertie salary*/
    public void setSalary(double salary){
        this.salary = salary;
    }//end method setSalary

//End GetterSetterExtension Source Code


}//End class
