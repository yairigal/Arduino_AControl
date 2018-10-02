#ifndef _datetimecontainer_h
#define _datetimecontainer_h


class DateTimeContainer{
  public:
   int selectedYear;
   int selectedMonth;
   int selectedDay;
   int selectedHour;
   int selectedMin;
   
   DateTimeContainer(int selectedYear, int selectedMonth, int selectedDay, int selectedHour, int selectedMin) {
        this->selectedYear = selectedYear;
        this->selectedMonth = selectedMonth;
        this->selectedDay = selectedDay;
        this->selectedHour = selectedHour;
        this->selectedMin = selectedMin;
    }
  DateTimeContainer() {} 

  bool after(DateTimeContainer next) {
        if (this->selectedYear > next.selectedYear)
            return true;
        else if (next.selectedYear > this->selectedYear)
            return false;
        else { // same year
            if (this->selectedMonth > next.selectedMonth)
                return true;
            else if (next.selectedMonth > this->selectedMonth)
                return false;
            else { // same month
                if (this->selectedDay > next.selectedDay)
                    return true;
                else if (next.selectedDay > this->selectedDay)
                    return false;
                else { // same day
                    if (this->selectedHour > next.selectedHour)
                        return true;
                    else if (this->selectedHour < next.selectedHour)
                        return false;
                    else {
                        return this->selectedMin > next.selectedMin;
                    }
                }
            }
        }

    }

    bool equals(DateTimeContainer next) {
        return this->selectedYear == next.selectedYear && this->selectedMonth == next.selectedMonth && this->selectedDay == next.selectedDay && this->selectedHour == next.selectedHour && this->selectedMin == next.selectedMin;
    }

    bool before(DateTimeContainer next) {
        return !this->equals(next) && !this->after(next);
    }
	
	String toString(){
		return this->selectedDay + String("/") + this->selectedMonth + String("/") + this->selectedYear + String(" ") + this->selectedHour + String(":") + this->selectedMin;
	}
  
};

class FutureAction{
  public:
    DateTimeContainer time;
    bool on;
};


#endif
