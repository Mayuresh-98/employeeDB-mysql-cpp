#include <iostream>
#include <stdlib.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

using namespace std;

// Database connection details
const string SERVER = "tcp://127.0.0.1:3306";
const string USERNAME = "root";
const string PASSWORD = "REMOVED";  // Change this
const string DATABASE = "employee_db";

class EmployeeDB {
private:
    sql::Driver* driver;
    sql::Connection* con;

public:
    // Constructor: Establishes database connection
    EmployeeDB() {
        try {
            driver = get_driver_instance();
            con = driver->connect(SERVER, USERNAME, PASSWORD);
            con->setSchema(DATABASE);
        }
        catch (sql::SQLException& e) {
            cout << "Database connection error: " << e.what() << endl;
            exit(1);
        }
    }

    // Destructor: Closes connection
    ~EmployeeDB() {
        delete con;
    }

    // Function to add an employee
    void addEmployee(string name, int age, string department, double salary) {
        sql::PreparedStatement* pstmt;
        try {
            pstmt = con->prepareStatement("INSERT INTO employees (name, age, department, salary) VALUES (?, ?, ?, ?)");
            pstmt->setString(1, name);
            pstmt->setInt(2, age);
            pstmt->setString(3, department);
            pstmt->setDouble(4, salary);
            pstmt->execute();
            cout << "Employee added successfully!\n";
            delete pstmt;
        }
        catch (sql::SQLException& e) {
            cout << "Error adding employee: " << e.what() << endl;
        }
    }

    // Function to view all employees
    void viewEmployees() {
        sql::Statement* stmt;
        sql::ResultSet* res;
        try {
            stmt = con->createStatement();
            res = stmt->executeQuery("SELECT * FROM employees");

            cout << "\nEmployee List:\n";
            while (res->next()) {
                cout << "ID: " << res->getInt("id")
                    << ", Name: " << res->getString("name")
                    << ", Age: " << res->getInt("age")
                    << ", Department: " << res->getString("department")
                    << ", Salary: " << res->getDouble("salary")
                    << endl;
            }
            delete res;
            delete stmt;
        }
        catch (sql::SQLException& e) {
            cout << "Error viewing employees: " << e.what() << endl;
        }
    }

    // Function to update employee details
    void updateEmployee(int id, string name, int age, string department, double salary) {
        sql::PreparedStatement* pstmt;
        try {
            pstmt = con->prepareStatement("UPDATE employees SET name=?, age=?, department=?, salary=? WHERE id=?");
            pstmt->setString(1, name);
            pstmt->setInt(2, age);
            pstmt->setString(3, department);
            pstmt->setDouble(4, salary);
            pstmt->setInt(5, id);
            int rows = pstmt->executeUpdate();

            if (rows > 0)
                cout << "Employee updated successfully!\n";
            else
                cout << "No employee found with ID " << id << endl;

            delete pstmt;
        }
        catch (sql::SQLException& e) {
            cout << "Error updating employee: " << e.what() << endl;
        }
    }

    // Function to delete an employee
    void deleteEmployee(int id) {
        sql::PreparedStatement* pstmt;
        try {
            pstmt = con->prepareStatement("DELETE FROM employees WHERE id=?");
            pstmt->setInt(1, id);
            int rows = pstmt->executeUpdate();

            if (rows > 0)
                cout << "Employee deleted successfully!\n";
            else
                cout << "No employee found with ID " << id << endl;

            delete pstmt;
        }
        catch (sql::SQLException& e) {
            cout << "Error deleting employee: " << e.what() << endl;
        }
    }
};

// Main function
int main() {
    EmployeeDB empDB;
    int choice;

    do {
        cout << "\nEmployee Management System\n";
        cout << "1. Add Employee\n";
        cout << "2. View Employees\n";
        cout << "3. Update Employee\n";
        cout << "4. Delete Employee\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        string name, department;
        int age, id;
        double salary;

        switch (choice) {
        case 1:
            cout << "Enter Name: ";
            cin >> name;
            cout << "Enter Age: ";
            cin >> age;
            cout << "Enter Department: ";
            cin >> department;
            cout << "Enter Salary: ";
            cin >> salary;
            empDB.addEmployee(name, age, department, salary);
            break;

        case 2:
            empDB.viewEmployees();
            break;

        case 3:
            cout << "Enter Employee ID to Update: ";
            cin >> id;
            cout << "Enter New Name: ";
            cin >> name;
            cout << "Enter New Age: ";
            cin >> age;
            cout << "Enter New Department: ";
            cin >> department;
            cout << "Enter New Salary: ";
            cin >> salary;
            empDB.updateEmployee(id, name, age, department, salary);
            break;

        case 4:
            cout << "Enter Employee ID to Delete: ";
            cin >> id;
            empDB.deleteEmployee(id);
            break;

        case 5:
            cout << "Exiting program...\n";
            break;

        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 5);

    return 0;
}
