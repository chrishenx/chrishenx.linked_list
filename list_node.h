/**
    Graph library
		Copyright (C) 2014  González León Christian

	  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	Contact the author: chrishendrix24@gmail.com
*/

#ifndef NODE_H
#define NODE_H

namespace chrishenx {

  template <typename T>
  class List;

  template <typename T>
  class ListNode {
    friend class List < T > ;
  public:
    ListNode(const T& val, ListNode* previous = nullptr, ListNode* next = nullptr)
      : value(val) {
      this->previous = previous;
      this->next = next;
    }
    ListNode(T&& val, ListNode* previous = nullptr, ListNode* next = nullptr)
      : value(std::move(val)) {
      this->previous = previous;
      this->next = next;
    }
  private:
    T value;
    ListNode* previous;
    ListNode* next;
  };

  // chrishenx namespace end
}
#endif // NODE_H
